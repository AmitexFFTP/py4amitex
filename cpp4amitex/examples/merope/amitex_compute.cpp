#include "amitex_compute.hpp"

#include "amitex/common.hpp"
#include "amitex/extract.hpp"
#include "amitex/field.hpp"
#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

namespace merope {
namespace amx {

template <typename Tid>
void makeZoneField(const Grid_VER& grid, Field<Tid>& ids) {
  NodesList::const_iterator pIt;

  // For each phase index id, save its value at each point in this phase
  unsigned short id;
  std::vector<NodesList>::const_iterator l;
  for (l = grid.getPhases().begin(), id = 0; l != grid.getPhases().end(); ++l, ++id) {
    for (pIt = l->begin(); pIt != l->end(); ++pIt) {
      auto i = grid.get_coord_index<3>(*pIt);
      // Not in bounds means voxel is owned by another process
      if (ids.inBounds(i))
        ids.uncheckedAt(i[0], i[1], i[2]) =
            id + 1;  // 1-based indexing to satify current convention
    }
  }
}

void makeZones(const Grid_VER& grid, Material& mat, const std::vector<double>& coefficients,
               int dir) {
  NodesList::const_iterator pIt;

  // For each phase index id, save its value at each point in this phase
  unsigned short id;
  std::vector<NodesList>::const_iterator l;
  for (l = grid.getPhases().begin(), id = 0; l != grid.getPhases().end(); ++l, ++id) {
    Zone zone{{grid.getNx(), grid.getNy(), grid.getNz()}};
    for (pIt = l->begin(); pIt != l->end(); ++pIt) {
      zone.add(grid.get_coord_index<3>(*pIt));
    }
    std::vector<double> coeffKs(4);
    coeffKs[0] = coefficients.at(id);
    for (size_t c = 1; c <= 3; c++) coeffKs[c] = dir + 1 == c ? -coeffKs[0] : 0.0;
    mat.addZone(zone, {}, coeffKs);
  }
  assert(mat.numberZones() == coefficients.size());
}

void setParametersAlgorithm(amx::Input& input) {
  amx::Algorithm algo;
  algo.type = "Basic_Scheme";
  algo.convergenceAcceleration = true;
  algo.convergenceCriterion = 1.e-4;
  algo.nitermax = 3000;
  input.algorithmParameters.algorithm = algo;
  amx::Diffusion diffu;
  diffu.filter = "Default";
  diffu.stationary = true;
  input.algorithmParameters.diffusion = diffu;
}

void setLoading(amx::Input& input) {
  amx::Loading load;
  double times[] = {1};
  load.setTimeDiscretizationUser(1, times);
  for (int i = 0; i < 3; i++) {
    load.setEvolution(i, amx::DiffusionDriving::Gradient, amx::Evolution::Linear, 0.0);
  }
  input.loadingOutput.add(std::move(load));
}

Matrix<3> computeThermalCoeff(const Grid_VER& grid, const std::vector<double>& coefficients,
                              int nbProcs) {
  Matrix<3> result;
  GridSize n{grid.getNx(), grid.getNy(), grid.getNz()};
  amx::Vector3D dx{
      {grid.getLx() / grid.getNx(), grid.getLy() / grid.getNy(), grid.getLz() / grid.getNz()}};
  size_t totalSize = grid.getNg();
  amx::Grid amxGrid{n, dx};
  amx::Input input{amxGrid};

  setParametersAlgorithm(input);
  setLoading(input);
  input.materials.referenceMaterialD =
      amx::ReferenceMaterialD{*std::max_element(coefficients.begin(), coefficients.end())};
  input.materials.setNumberMaterials(1);

  for (size_t direction = 0; direction < 3; direction++) {
    input.resultsDir = "result_folder/" + std::to_string(direction);

    amx::Material material;
    material.setLawK("Fourier_iso_polarization");
    material.setNumberCoeffK(4);
    makeZones(grid, material, coefficients, direction);

    // Reset material
    input.materials.material(0) = material;

    runSimulationExternal(input, nbProcs);

    amx::Extract ext{input.outputPrefix()};
    auto flux = ext.averageDiffusionFlux(0);
    for (size_t i = 0; i < 3; i++) result[i][direction] = -flux[i];
  }
  return result;
}

}  // namespace amx
}  // namespace merope
