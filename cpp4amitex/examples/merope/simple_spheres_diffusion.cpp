#include <iostream>
#include <string>

#include "MultiInclusions/MultiInclusions.hxx"
#include "MultiInclusions/SphereInclusions.hxx"
#include "Voxellation/Voxellation.hxx"

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

static void makeZones(const merope::Grid_VER& grid, amitex::Material& mat,
                      const std::vector<std::vector<double>>& coefficients,
                      const std::vector<std::vector<double>>& coefficientsK);

amitex::Material makeMaterialDiffusion(const merope::vox::Voxellation<3>& voxelation,
                                       const std::pair<std::string, std::string>& law) {
  amitex::Material material;
  material.setLawK(law.first, law.second);
  material.setNumberCoeffK(1);
  makeZones(voxelation.getGrid(), material, {}, {voxelation.getCoefficents()});
  return material;
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


int main() {
  using namespace merope;
  using namespace sac_de_billes;
  using amitex::Component;
  using amitex::DiffusionDriving;
  using namespace std::string_literals;

  size_t n = 32;
  double dx = 10. / n;
  auto sphIncl = SphereInclusions<3>();
  sphIncl.setLength({n * dx, n * dx, n * dx});
  sphIncl.fromHisto(0, algoSpheres::TypeAlgo::RSA, 0.0, {{3, 0.5}}, {1});

  auto multiInclusions = MultiInclusions<3>();
  multiInclusions.setInclusions(sphIncl);
  auto grid = vox::Voxellation<3>(multiInclusions);
  grid.setPureCoeffs({1.0, 3.0});
  grid.setHomogRule(homogenization::Rule::Voigt);
  grid.setVoxelRule(vox::VoxelRule::Average);
  grid.proceed({n, n, n});

  amitex::Input input;
  input.resultsDir = "amitex_merope_spheres_with_build_mat";
  input.grid = amitex::Grid{{n, n, n}, {dx, dx, dx}};

  input.algorithmParameters = defaultDiffusionAlgorithm();

  amitex::Loading loading;
  loading.setTimeDiscretizationLinear(1, 1.);
  loading.setLinearEvolution(Component::X, DiffusionDriving::Gradient, 1.);
  loading.setLinearEvolution(Component::Y, DiffusionDriving::Gradient, 0.);
  loading.setLinearEvolution(Component::Z, DiffusionDriving::Gradient, 0.);
  input.loadingOutput.add(loading);

  input.materials.referenceMaterialD = amitex::ReferenceMaterialD{3.};

  amitex::Material material = makeMaterialDiffusion(grid, {"Fourier_iso"s, ""s});
  input.materials.add(material);

  amitex::runSimulationExternal(input, 2);

  amitex::Extract ext{input.outputPrefix()};
  auto flux = ext.averageDiffusionFlux(0);
  auto grad = ext.averageDiffusionGradient(0);
  std::cout << "Flux\tGradient\n";
  for (size_t i = 0; i < 3; i++) std::cout << flux[i] << '\t' << grad[i] << '\n';
}

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
