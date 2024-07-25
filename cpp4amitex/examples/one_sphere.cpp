#include <iostream>

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

using namespace amitex;

int main() {
  size_t NX = 32, NY = 32, NZ = 32;
  double DX = 0.0003125, DY = 0.0003125, DZ = 0.0003125;
  double R = 0.3 * DX * NX;
  std::vector<double> kappas{0.6, 429.0};
  GridSize nx{NX, NX, NZ};
  Vector3D dx{DX, DY, DZ};

  Grid grid{nx, dx};

  Materials materials;
  materials.referenceMaterialD = ReferenceMaterialD(214.8);
  Zone zone0{grid.dims()}, zone1{grid.dims()};
  GridPoint center = {NX / 2, NY / 2, NZ / 2};
  for (auto p : grid.allPoints()) {
    if (grid.distance(p, center) < R) {
      zone1.add(p);
    } else {
      zone0.add(p);
    }
  }

  Material mat;
  mat.setLawK("Fourier_iso_polarization");
  mat.setNumberCoeffK(4);

  mat.addZone(std::move(zone0), {}, {kappas[0], 0.0, 0.0, -kappas[0]});
  mat.addZone(std::move(zone1), {}, {kappas[1], 0.0, 0.0, -kappas[1]});

  materials.add(std::move(mat));

  Algorithm algo{"Basic_Scheme", true};
  algo.convergenceCriterion = 1.e-4;
  algo.nitermax = 3000;

  Diffusion diffu;
  diffu.filter = "Default";
  diffu.stationary = true;
  AlgorithmParameters algoParams{algo};
  algoParams.diffusion = diffu;

  LoadingOutput loading;
  Loading load;
  load.setTimeDiscretizationLinear(1, 1.0);
  for (int i = 0; i < 3; i++) {
    load.setEvolution(i, DiffusionDriving::Gradient, Evolution::Linear, 0.0);
  }
  loading.add(std::move(load));

  Input input{grid, std::move(algoParams), std::move(materials), std::move(loading)};
  input.resultsDir = "amitex_dir_one_sphere";

  runSimulationExternal(input);

  Extract ext{input.outputPrefix()};
  auto flux = ext.averageDiffusionFlux(0);
  auto gradT = ext.averageDiffusionGradient(0);
  std::cout << "Flux\tGradient\n";
  for (size_t i = 0; i < 3; i++) std::cout << flux[i] << '\t' << gradT[i] << '\n';
}