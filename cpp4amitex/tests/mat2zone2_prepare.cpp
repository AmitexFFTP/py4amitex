#include "mat2zone2_prepare.hpp"

#include <cassert>

namespace amitex_tests {

amitex::Ptr<amitex::Input> mat2zone2Prepare(int nbMat) {
  using namespace amitex;
  assert(nbMat >= 1 && nbMat <= 2);
  Grid grid{{32, 32, 32}, {0003125, 0003125, 0003125}};
  double kappas[] = {0.6, 429};

  auto algo = Algorithm::create("Basic_Scheme", true);
  auto diffu = Diffusion::create("Default", true);
  auto algoParams = AlgorithmParameters::create(algo);
  algoParams->diffusion = diffu;

  auto loadingOutput = LoadingOutput::create();
  auto init = InitLoadExt::create();
  init->temperature = 300;
  init->setParam(0, 0);
  loadingOutput->initLoadExt = init;

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(1, 1.0);
  loading->setLinearEvolution(Component::X, DiffusionDriving::Gradient, 0.0);
  loading->setLinearEvolution(Component::Y, DiffusionDriving::Gradient, 0.0);
  loading->setLinearEvolution(Component::Z, DiffusionDriving::Gradient, 0.0);
  loading->setTemperatureEvolution(Evolution::Constant);
  loading->setParamEvolution(0, Evolution::Linear, 1);

  loadingOutput->add(loading);

  auto materials = Materials::create();
  materials->referenceMaterialD = ReferenceMaterialD::create(214.8);

  auto [NX, NY, NZ] = grid.dims();
  auto [DX, DY, DZ] = grid.voxelLengths();
  double R = 0.3 * DX * NX;
  Vector3D center = {(NX * DX) / 2, (NY * DY) / 2, (NZ * DZ) / 2};
  auto zone0 = Zone::create(grid.dims()), zone1 = Zone::create(grid.dims());
  for (auto point : grid.allPoints()) {
    if (grid.distance(point, center) < R) {
      zone1->add(point);
    } else {
      zone0->add(point);
    }
  }

  for (size_t m = 0; m < nbMat; m++) {
    auto mat = Material::create();
    mat->setLawK("Fourier_iso_polarization");
    mat->setNumberCoeffK(4);

    if (nbMat == 1) {
      mat->addZone(zone0, {}, {kappas[0], 0.0, 0.0, -kappas[0]});
      mat->addZone(zone1, {}, {kappas[1], 0.0, 0.0, -kappas[1]});
    } else if (m == 0) {
      mat->addZone(zone0, {}, {kappas[0], 0.0, 0.0, -kappas[0]});
    } else {
      mat->addZone(zone1, {}, {kappas[1], 0.0, 0.0, -kappas[1]});
    }

    materials->add(mat);
  }
  auto input = Input::create(grid, algoParams, materials, loadingOutput);
  input->resultsDir = "testresults/addzoneandlaunchthermal_" + std::to_string(nbMat);
  return input;
}

}  // namespace amitex_tests