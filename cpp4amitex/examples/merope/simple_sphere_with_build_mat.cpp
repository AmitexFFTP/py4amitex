#include <iostream>
#include <string>

#include "MultiInclusions/MultiInclusions.hxx"
#include "MultiInclusions/SphereInclusions.hxx"
#include "Voxellation/Voxellation.hxx"

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

#include "amitex_make_input.hpp"

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
