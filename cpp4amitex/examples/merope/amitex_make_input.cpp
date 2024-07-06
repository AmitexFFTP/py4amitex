
#include <algorithm>

#include "Voxellation/Voxellation.hxx"
#include "amitex/algorithm_parameters.hpp"
#include "amitex/errors.hpp"
#include "amitex/materials.hpp"

static void makeZones(const merope::Grid_VER& grid, amitex::Material& mat,
                      const std::vector<std::vector<double>>& coefficients,
                      const std::vector<std::vector<double>>& coefficientsK) {
  using amitex::Zone;
  using namespace merope;
  NodesList::const_iterator pIt;

  // For each phase index id, save its value at each point in this phase
  unsigned short id;
  std::vector<NodesList>::const_iterator l;
  for (l = grid.getPhases().begin(), id = 0; l != grid.getPhases().end(); ++l, ++id) {
    Zone zone{{grid.getNx(), grid.getNy(), grid.getNz()}};
    for (pIt = l->begin(); pIt != l->end(); ++pIt) {
      zone.add(grid.get_coord_index<3>(*pIt));
    }
    std::vector<double> coeffs(coefficients.size());
    for (size_t c = 0; c < coefficients.size(); c++) coeffs[c] = coefficients[c].at(id);
    std::vector<double> coeffKs(coefficientsK.size());
    for (size_t c = 0; c < coefficientsK.size(); c++) coeffKs[c] = coefficientsK[c].at(id);
    mat.addZone(zone, coeffs, coeffKs);
  }
}

amitex::Material makeMaterialDiffusion(const merope::vox::Voxellation<3>& voxelation,
                                       const std::pair<std::string, std::string>& law) {
  amitex::Material material;
  material.setLawK(law.first, law.second);
  material.setNumberCoeffK(1);
  makeZones(voxelation.getGrid(), material, {}, {voxelation.getCoefficents()});
  return material;
}

using Phase = std::tuple<merope::vox::VTK_PHASE, double>;

static void makeZonesComposite(const std::vector<std::vector<Phase>>& phases,
                               const merope::Grid_VER& grid, const std::string& law,
                               amitex::MaterialComposite& composites, amitex::Zone& zoneInter,
                               std::vector<amitex::Zone>& zonePures) {
  using namespace amitex;
  using std::get;
  GridSize gridDims = {grid.getNx(), grid.getNy(), grid.getNz()};
  std::map<std::vector<size_t>, size_t> compos;
  for (size_t p = 0; p < phases.size(); p++) {
    GridPoint pos = grid.get_coord_index<3>(p);
    GridLinPoint lpos = linearize(pos, gridDims);
    // Order of phase matte to avoid duplicates
    std::vector<Phase> phase = phases[p];
    std::sort(phase.begin(), phase.end(), [](Phase a, Phase b) { return get<0>(a) < get<0>(b); });
    std::vector<size_t> phaseIds;
    std::vector<double> volFracs;
    // Filer rare phases
    for (const auto& p : phase) {
      double vf = get<1>(p);
      if (vf > Composite::maxVolumeFraction) {
        phaseIds.push_back(get<0>(p));
        volFracs.push_back(vf);
      }
    }
    if (phaseIds.size() == 0) throw InputError{"phase volume fraction likely wrong"};
    if (phaseIds.size() == 1) {
      size_t mid = phaseIds[0];
      if (mid >= zonePures.size()) {
        for (size_t m = zonePures.size(); m <= mid; m++) zonePures.emplace_back(Zone{gridDims});
      }
      zonePures.at(mid).add(lpos);
    } else {
      zoneInter.add(lpos);
      auto it = compos.find(phaseIds);
      if (it == compos.end()) {
        auto [it2, ok] = compos.insert({phaseIds, composites.numberMaterials()});
        if (!ok) throw InputError{std::string{__func__} + " map could not insert"};
        composites.add(Composite{phaseIds, law});
        it = it2;
      }
      size_t ic = it->second;
      composites.at(ic).addVoxel(lpos, volFracs);
    }
  }
}

void makeCompositeMaterials(amitex::Materials& materials, merope::vox::Voxellation<3>& voxelation,
                            const std::string& law) {
  using namespace amitex;
  const auto& grid = voxelation.getGrid();
  GridSize gridDims = {grid.getNx(), grid.getNy(), grid.getNz()};

  std::vector<Zone> zonePures;
  Zone zoneInter{gridDims};
  makeZonesComposite(voxelation.computePhaseGrid(gridDims), grid, law, materials.composites,
                     zoneInter, zonePures);

  if (materials.numberMaterials() < zonePures.size())
    materials.setNumberMaterials(zonePures.size());

  for (size_t m = 0; m < zonePures.size(); m++) {
    materials.at(m).addZone(std::move(zonePures[m]));
  }
  materials.at(0).addZone(zoneInter);
}

amitex::AlgorithmParameters defaultDiffusionAlgorithm() {
  using namespace amitex;
  AlgorithmParameters algo_params;
  Algorithm algorithm;
  algorithm.convergenceAcceleration = true;
  algorithm.nitermax = 3000;
  Diffusion diffusion;
  diffusion.filter = "Default";
  diffusion.stationary = true;
  algo_params.algorithm = algorithm;
  algo_params.diffusion = diffusion;
  return algo_params;
}

amitex::AlgorithmParameters defaultMechanicsAlgorithm() {
  using namespace amitex;
  AlgorithmParameters algo_params;
  Algorithm algorithm;
  algorithm.convergenceAcceleration = true;
  Mechanics mechanics;
  mechanics.filter = "Default";
  mechanics.smallPerturbations = true;
  algo_params.algorithm = std::move(algorithm);
  algo_params.mechanics = std::move(mechanics);
  return algo_params;
}