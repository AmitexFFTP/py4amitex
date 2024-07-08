#include <array>
#include <iostream>

#include "MultiInclusions/MultiInclusions.hxx"
#include "MultiInclusions/SphereInclusions.hxx"
#include "Voxellation/Voxellation.hxx"

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/input/material_builder.hpp"
#include "amitex/simulation.hpp"

using VtkFormatAnIso = merope::vox::composite::vtk_format_anIso<3>;

std::vector<VtkFormatAnIso> calcMicro(std::array<size_t, 3> N, std::array<double, 3> L) {
  using namespace merope;
  using namespace sac_de_billes;
  auto sphIncl = SphereInclusions<3>();
  sphIncl.setLength({10, 10, 10});
  sphIncl.fromHisto(0, algoSpheres::TypeAlgo::RSA, 0.0, {{3, 0.5}}, {1});

  auto multiInclusions = MultiInclusions<3>();
  multiInclusions.setInclusions(sphIncl);
  auto grid = vox::Voxellation<3>(multiInclusions);

   grid.setVoxelRule(vox::VoxelRule::Laminate);
   return grid.computeCompositeGrid(N);
}

int sim() {
  using namespace amitex;
  std::array<size_t, 3> N = {32, 32, 32};
  std::array<double, 3> L = {10.0, 10, 10};

  auto compGrid = calcMicro(N, L);
  double DL = L[0] / N[0];
  Input input;
  Grid grid{N, {DL, DL, DL}};
  input.grid = grid;

  Materials materials;
  buildMaterials(materials, grid.dims(), compGrid, IndexOrdering::C);

  std::vector<std::vector<double>> coeffs = {{1.0, 2.0}, {1.0, 2.0}};

  for (size_t i = 0; i < materials.numberMaterials(); i++) {
    std::cout << materials.material(i).numberZones() << "\n";
    for (const auto& zone : materials.material(i).zones()) {
      std::cout << "Mat " << i << " #zones = " << zone.numberVoxels() << "\n";
    }
    materials.material(i).setLaw("elasiso");
    materials.material(i).setCoeffs(coeffs[i]);
    materials.material(i).setCoeffComposites(coeffs[i]);
  }

  for (size_t i = 0; i < materials.numberComposites(); i++) {
    materials.composite(i).setLaw("laminate");
    std::cout << "Composite " << i << " #vox=" << materials.composite(i).positions().size();
  }

  materials.referenceMaterial =
      ReferenceMaterial{0.5 * (coeffs[0][0] + coeffs[1][0]), 0.5 * (coeffs[0][1] + coeffs[1][1])};
  input.materials = std::move(materials);

  Algorithm algo;
  algo.type = "Basic_Scheme";
  algo.convergenceAcceleration = true;
  Mechanics meca;
  meca.filter = "Default";
  meca.smallPerturbations = true;
  input.algorithmParameters.mechanics = meca;
  input.algorithmParameters.algorithm = algo;

  Loading load;
  load.setTimeDiscretizationUser({1e-3, 2e-3, 1.e-2});
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++)
      load.setEvolution({i, j}, MechanicDriving::Stress, Evolution::Linear, 0.0);
  }
  load.setEvolution(Component::XX, MechanicDriving::Strain, Evolution::Linear, 0.01);
  input.loadingOutput.add(std::move(load));

  input.resultsDir = "amitex_dir_simple_spheres";
  input.generateFiles();
  runSimulationExternal(input);

  Extract ext{input.outputPrefix()};
  auto sig = ext.averageStress();
  for (auto row : sig) {
    std::cout << row[0] << "\t" << row[1] << "\t" << row[2] << "\n";
  }
  auto def = ext.averageStrain();
  for (auto row : def) {
    std::cout << row[0] << "\t" << row[1] << "\t" << row[2] << "\n";
  }
  return 0;
}

int main() { return sim(); }
