#include <iostream>

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
  using amitex::MechanicDriving;

  double lambda[] = {1.e9, 2.e9};
  double mu[] = {3.e8, 4.e8};
  size_t n = 32;
  double dx = 10. / n;
  auto sphIncl = SphereInclusions<3>();
  sphIncl.setLength({n * dx, n * dx, n * dx});
  sphIncl.fromHisto(0, algoSpheres::TypeAlgo::RSA, 0.0, {{3, 0.5}}, {1});

  auto multiInclusions = MultiInclusions<3>();
  multiInclusions.setInclusions(sphIncl);
  auto grid = vox::Voxellation<3>(multiInclusions);
  // grid.setPureCoeffs({1., 1.});
  grid.setHomogRule(homogenization::Rule::Voigt);
  grid.setVoxelRule(vox::VoxelRule::Average);
  grid.proceed({n, n, n});

  amitex::Input input;
  input.resultsDir = "amitex_merope_spheres_with_build_mat_composite";
  input.grid = amitex::Grid{{n, n, n}, {dx, dx, dx}};

  input.algorithmParameters = defaultMechanicsAlgorithm();

  amitex::Loading loading;
  loading.setTimeDiscretizationLinear(1, 1.);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) loading.setLinearEvolution({i, j}, MechanicDriving::Stress, 0.);
  }
  loading.setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.01);
  input.loadingOutput.add(loading);

  for (size_t m = 0; m < 2; m++) {
    amitex::Material material;
    material.setLaw("elasiso");
    material.setCoeffs({lambda[m], mu[m]});
    material.setCoeffComposites({lambda[m], mu[m]});
    input.materials.add(material);
  }
  makeCompositeMaterials(input.materials, grid, "voigt");

  input.materials.referenceMaterial =
      amitex::ReferenceMaterial{0.5 * (lambda[0] + lambda[1]), 0.5 * (mu[0] + mu[1])};

  amitex::runSimulationExternal(input, 1);

  amitex::Extract ext{input.outputPrefix()};
  auto def = ext.averageStrain();
  auto sig = ext.averageStress();
  std::cout << "Strain\n";
  for (auto row : def) std::cout << row[0] << '\t' << row[1] << '\t' << row[2] << '\n';
  std::cout << "Stress\n";
  for (auto row : sig) std::cout << row[0] << '\t' << row[1] << '\t' << row[2] << '\n';
}
