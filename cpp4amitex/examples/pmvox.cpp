#include "pmvox.hpp"

#include <vector>

amitex::Input make_thermo_pmvox_input_gen(std::string_view resultsDir, int nmat) {
  using namespace amitex;
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
  for (size_t ix = 0; ix < NZ; ix++) {
    for (size_t iy = 0; iy < NY; iy++) {
      for (size_t iz = 0; iz < NZ; iz++) {
        auto dx = (ix * 1. - NX / 2) * DX;
        auto dy = (iy * 1. - NY / 2) * DY;
        auto dz = (iz * 1. - NZ / 2) * DZ;
        auto sep2 = dx * dx + dy * dy + dz * dz;
        if (sep2 < R * R) {
          zone1.add({ix, iy, iz});
        } else {
          zone0.add({ix, iy, iz});
        }
      }
    }
  }

  for (size_t m = 0; m < nmat; m++) {
    Material mat;
    mat.setLawK("Fourier_iso_polarization");
    mat.setNumberCoeffK(4);

    if (nmat == 1) {
      mat.addZone(zone0, {}, {kappas[0], 0.0, 0.0, -kappas[0]});
      mat.addZone(zone1, {}, {kappas[1], 0.0, 0.0, -kappas[1]});
    } else if (m == 0) {
      mat.addZone(zone0, {}, {kappas[0], 0.0, 0.0, -kappas[0]});
    } else {
      mat.addZone(zone1, {}, {kappas[1], 0.0, 0.0, -kappas[1]});
    }

    materials.add(mat);
  }

  Algorithm algo{"Basic_Scheme", true};
  algo.convergenceCriterion = 1.e-4;
  algo.nitermax = 3000;
  Diffusion diffu;
  diffu.filter = "Default";
  diffu.stationary = true;
  AlgorithmParameters algorithmParameters{algo};
  algorithmParameters.diffusion = diffu;

  LoadingOutput loading;
  Loading load;
  double times[] = {1, 2};
  load.setTimeDiscretizationUser(2, times);
  for (int i = 0; i < 3; i++) {
    load.setEvolution(i, DiffusionDriving::Gradient, Evolution::Linear, 0.0);
  }
  loading.add(std::move(load));

  Input input{grid, algorithmParameters, materials, loading};

  input.resultsDir = resultsDir;

  return input;
}

amitex::Input make_thermo_pmvox_input(std::string_view resultsDir) {
  return make_thermo_pmvox_input_gen(resultsDir, 2);
}
