#include <array>
#include <iostream>

#include "MultiInclusions/MultiInclusions.hxx"
#include "MultiInclusions/SphereInclusions.hxx"
#include "Voxellation/DynamicVoxellizer.hxx"

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/input/material_builder.hpp"
#include "amitex/simulation.hpp"

using VtkFormatAnIso = merope::vox::composite::stl_format_AnIso<3, long>;

auto calcMicro(std::array<size_t, 3> N, std::array<double, 3> L) {
  using namespace merope;
  using namespace sac_de_billes;
  auto sphIncl = SphereInclusions<3>();
  sphIncl.setLength({10, 10, 10});
  sphIncl.fromHisto(0, algoSpheres::TypeAlgo::RSA, 0.0, {{3, 0.5}}, {1});

  auto multiInclusions = MultiInclusions<3>();
  multiInclusions.setInclusions(sphIncl);
  auto structure = Structure<3>{multiInclusions};
  auto grid = vox::voxellizer::GridRepresentation<3>{
      structure, vox::create_grid_parameters_N_L<3>(N, L), vox::VoxelRule::Laminate};

  grid.convert_to_stl_format();
  return grid;
}

int sim() {
  using namespace amitex;
  std::array<size_t, 3> N = {32, 32, 32};
  std::array<double, 3> L = {10.0, 10, 10};

  // auto compGrid = calcMicro(N, L);
  auto mgrid = calcMicro(N, L).get<VtkFormatAnIso>();
  // exit(0);
  const std::vector<VtkFormatAnIso>& compGrid = mgrid;
  double DL = L[0] / N[0];
  Grid grid{N, {DL, DL, DL}};

  auto materials = Materials::create();
  buildMaterials(*materials, grid.dims(), compGrid, IndexOrdering::C);
  std::vector<std::vector<double>> coeffs = {{1.0, 2.0}, {1.0, 2.0}};

  for (size_t i = 0; i < materials->numberMaterials(); i++) {
    std::cout << materials->material(i)->numberZones() << "\n";
    for (const auto& zone : materials->material(i)->zones()) {
      std::cout << "Mat " << i << " #voxels = " << zone->numberVoxels() << "\n";
    }
    materials->material(i)->setLaw("elasiso");
    materials->material(i)->setCoeffs(coeffs[i]);
    materials->material(i)->setCoeffComposites(coeffs[i]);
  }

  for (size_t i = 0; i < materials->numberComposites(); i++) {
    materials->composite(i)->setLaw("laminate");
    std::cout << "Composite " << i << " #voxels = " << materials->composite(i)->positions().size()
              << '\n';
  }

  materials->referenceMaterial = ReferenceMaterial::create(0.5 * (coeffs[0][0] + coeffs[1][0]),
                                                           0.5 * (coeffs[0][1] + coeffs[1][1]));

  auto algo = Algorithm::create("Basic_Scheme", true);
  auto meca = Mechanics::create("Default", true);

  auto load = Loading::create();
  load->setTimeDiscretizationUser({1e-3, 2e-3, 1.e-2});
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++)
      load->setEvolution({i, j}, MechanicDriving::Stress, Evolution::Linear, 0.0);
  }
  load->setEvolution(Component::XX, MechanicDriving::Strain, Evolution::Linear, 0.01);
  auto loadingOutput = LoadingOutput::create();
  loadingOutput->add(load);

  auto input =
      Input::create(grid, AlgorithmParameters::create(algo, meca), materials, loadingOutput);

  input->resultsDir = "amitex_dir_simple_spheres";
  runSimulationExternal(*input);

  Extract ext{input->outputPrefix()};
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
