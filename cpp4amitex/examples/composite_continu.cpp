#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/io.hpp"
#include "amitex/simulation.hpp"

using namespace amitex;

// Sketch of phase 1 Φ (along XY plane)
// |...……...|
// |...……...|
// |...……...|
// |...……...|

constexpr std::array pureCoeffs0 = {100e6, 200e6};
constexpr std::array pureCoeffs1 = {150e6, 250e6};

constexpr GridSize dims = {32, 32, 32};
constexpr Vector3D voxL = {1.0, 1.0, 1.0};

static Input makeInputCommon() {
  Grid grid{dims, voxL};
  Algorithm algo{"Basic_Scheme", true};
  Mechanics meca;
  meca.filter = "Default";
  meca.smallPerturbations = true;
  AlgorithmParameters paramAlgo{std::move(algo)};
  paramAlgo.mechanics = std::move(meca);

  Loading loading;
  loading.setTimeDiscretizationLinear(4, 1.0);
  // Something along the interface
  loading.setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.01);
  loading.setLinearEvolution(Component::XY, MechanicDriving::Strain, 0.01);
  loading.setLinearEvolution(Component::YY, MechanicDriving::Strain, 0.01);
  loading.setLinearEvolution(Component::XZ, MechanicDriving::Stress, 0.0);
  loading.setLinearEvolution(Component::YZ, MechanicDriving::Stress, 0.0);
  loading.setLinearEvolution(Component::ZZ, MechanicDriving::Stress, 0.0);
  LoadingOutput lo;
  lo.add(std::move(loading));

  Materials materials;
  materials.referenceMaterial = ReferenceMaterial{0.5 * (pureCoeffs0[0] + pureCoeffs0[1]),
                                                  0.5 * (pureCoeffs1[0] + pureCoeffs1[1])};
  return Input{grid, std::move(paramAlgo), std::move(materials), std::move(lo)};
}

static std::pair<std::vector<size_t>, std::vector<double>> getPhasesVolfrac(GridPoint p,
                                                                            GridSize dims) {
  double L = dims[0] * voxL[0];
  double x = p[0] * voxL[0];
  double x0 = 0.5 * L;
  double l0 = 0.1 * L;
  double phi[2];
  double f = (x - x0) / l0;
  phi[1] = 0.95 * exp(-0.5 * f * f);
  phi[0] = 1.0 - phi[1];
  assert(phi[1] >= 0.0 && phi[1] <= 1.0);

  std::vector<size_t> pids;
  std::vector<double> vfs;
  for (size_t i = 0; i < 2; i++) {
    if (phi[i] > 1.0e-3) {
      pids.push_back(i);
      vfs.push_back(phi[i]);
    }
  }
  return {pids, vfs};
}

static Tensor3D composite_prism(const std::string& law) {
  Zone zone0{dims}, zone1{dims}, zoneInter{dims};
  Input input = makeInputCommon();
  std::ostringstream resultDir;
  resultDir << "testresults/compositeconti_";
  resultDir << law;
  input.resultsDir = resultDir.str();

  for (size_t m = 0; m < 2; m++) {
    Material mat;
    mat.setLaw("elasiso");
    mat.setCoeffs({pureCoeffs0[m], pureCoeffs1[m]});
    mat.setCoeffComposites({pureCoeffs0[m], pureCoeffs1[m]});
    input.materials.add(mat);
  }

  Field<double> vf0{dims};

  InterfaceGeometry geom{
      .normal = {-1, 0.0, 0.0}, .tangent = {0, 1., 0.}, .surface = voxL[1] * voxL[2]};
  Composite inter{{0, 1}, law};
  for (size_t ix = 0; ix < dims[0]; ix++) {
    for (size_t iy = 0; iy < dims[1]; iy++) {
      for (size_t iz = 0; iz < dims[2]; iz++) {
        GridPoint pos = {ix, iy, iz};
        GridLinPoint lpos = input.grid.linearize(pos);
        auto [pids, vfs] = getPhasesVolfrac(pos, dims);
        if (pids.size() == 1) {
          if (pids[0] == 0) {
            zone0.add(lpos);
            vf0[pos] = 1;
          } else {
            zone1.add(lpos);
            vf0[pos] = 1;
          }
        } else {
          vf0[pos] = vfs[0];
          inter.addVoxel(lpos, vfs, {geom});
          zoneInter.add(lpos);
        }
      }
    }
  }
  std::filesystem::create_directories("testresults");
  writeVTK("testresults/vf0.vtk", dims, voxL, vf0.data(), input.grid.totalSize());

  input.materials.material(0).addZone(zone0);
  input.materials.material(1).addZone(zone1);
  std::cerr << "#voxels z0 " << zone0.linearPositions().size() << "\n";
  std::cerr << "#voxels z1 " << zone1.linearPositions().size() << "\n";

  input.materials.material(0).addZone(zoneInter);
  // input.materials.material(1).addZone(zoneInter);

  std::cerr << "#zones m0 " << input.materials.material(0).numberZones() << "\n";
  std::cerr << "#zones m1 " << input.materials.material(1).numberZones() << "\n";

  input.materials.composites.add(std::move(inter));

  runSimulationExternal(input, 2);

  Extract ext{input.outputPrefix()};
  return ext.averageStress();
}

static void compare() {
  Tensor3D stressCompReuss = composite_prism("reuss");
  Tensor3D stressCompVoigt = composite_prism("voigt");
  Tensor3D stressCompLaminate = composite_prism("laminate");
  std::cerr << "Reuss         " << "Voigt         " << "Laminate      " << "\n";
  for (size_t i = 0; i < 3; i++) {
    for (size_t j = i; j < 3; j++) {
      std::cerr << std::setw(14) << stressCompReuss[i][j] << std::setw(14) << stressCompVoigt[i][j]
                << std::setw(14) << stressCompLaminate[i][j] << "\n";
    }
  }
}

int main() { compare(); }
