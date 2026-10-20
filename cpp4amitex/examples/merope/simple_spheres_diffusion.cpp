#include <iostream>
#include <string>

#include <map>

#include "Grid/Grid_VER.hxx"
#include "MultiInclusions/MultiInclusions.hxx"
#include "MultiInclusions/SphereInclusions.hxx"
#include "Voxellation/DynamicVoxellizer.hxx"

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

amitex::Ptr<amitex::Material> makeMaterialDiffusion(
    std::array<size_t, 3> N, const merope::vox::voxellizer::GridRepresentation<3>& voxelation,
    const std::pair<std::string, std::string>& law) {
  auto material = amitex::Material::create();
  material->setLawK(law.first, law.second);
  material->setNumberCoeffK(1);
  const auto& coeffs = voxelation.get<merope::vox::composite::Pure<double>>();

  std::map<double, std::vector<std::array<size_t, 3>>> uniqueCoeffs;

  for (size_t i = 0; i < N[0]; i++) {
    for (size_t j = 0; j < N[1]; j++) {
      for (size_t k = 0; k < N[2]; k++) {
        size_t id = k + N[2] * (j + N[1] * i);
        uniqueCoeffs[coeffs[id]].push_back({i, j, k});
      }
    }
  }
  for (const auto& [coeff, pos] : uniqueCoeffs) {
    auto zone = amitex::Zone::create(N, pos);
    material->addZone(zone, {}, {coeff});
  }
  std::cout << "#zones = " << material->numberZones() << '\n';
  return material;
}

amitex::Ptr<amitex::AlgorithmParameters> defaultDiffusionAlgorithm() {
  using namespace amitex;
  auto algorithm = Algorithm::create("Default", true);
  auto diffusion = Diffusion::create("Default", true);
  auto algo_params = AlgorithmParameters::create(algorithm);
  algo_params->diffusion = diffusion;
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
  std::array<size_t, 3> N = {n, n, n};
  std::array<double, 3> L = {n * dx, n * dx, n * dx};
  auto sphIncl = SphereInclusions<3>();
  sphIncl.setLength(L);
  sphIncl.fromHisto(0, algoSpheres::TypeAlgo::RSA, 0.0, {{3, 0.5}}, {1});

  auto multiInclusions = MultiInclusions<3>();
  multiInclusions.setInclusions(sphIncl);
  auto structure = Structure<3>{multiInclusions};
  auto grid = vox::voxellizer::GridRepresentation<3>{
      structure, vox::create_grid_parameters_N_L<3>(N, L), vox::VoxelRule::Average};
  grid.apply_coefficients({1.0, 3.0});
  grid.apply_homogRule(homogenization::Rule::Voigt);
  // grid.proceed({n, n, n});

  auto loading = amitex::Loading::create();
  loading->setTimeDiscretizationLinear(1, 1.);
  loading->setLinearEvolution(Component::X, DiffusionDriving::Gradient, 1.);
  loading->setLinearEvolution(Component::Y, DiffusionDriving::Gradient, 0.);
  loading->setLinearEvolution(Component::Z, DiffusionDriving::Gradient, 0.);
  auto loadingOutput = amitex::LoadingOutput::create();
  loadingOutput->add(loading);

  auto material = makeMaterialDiffusion(N, grid, {"Fourier_iso"s, ""s});
  auto materials = amitex::Materials::create();
  materials->add(material);
  materials->referenceMaterialD = amitex::ReferenceMaterialD::create(3.);

  auto input = amitex::Input::create(amitex::Grid{{n, n, n}, {dx, dx, dx}},
                                     defaultDiffusionAlgorithm(), materials, loadingOutput);
  input->resultsDir = "amitex_merope_spheres_with_build_mat";

  amitex::runSimulationExternal(*input, 2);

  amitex::Extract ext{input->outputPrefix()};
  auto flux = ext.averageDiffusionFlux(0);
  auto grad = ext.averageDiffusionGradient(0);
  std::cout << "Flux\tGradient\n";
  for (size_t i = 0; i < 3; i++) std::cout << flux[i] << '\t' << grad[i] << '\n';
}
