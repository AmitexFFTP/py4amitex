#include <iostream>

#include "MultiInclusions/MultiInclusions.hxx"
#include "MultiInclusions/SphereInclusions.hxx"
#include "Voxellation/Voxellation.hxx"

#include "amitex_compute.hpp"

using namespace merope;
using namespace sac_de_billes;

int main() {
  auto sphIncl = SphereInclusions<3>();
  sphIncl.setLength({10, 10, 10});
  sphIncl.fromHisto(0, algoSpheres::TypeAlgo::RSA, 0.0, {{3, 0.5}}, {1});

  auto multiInclusions = MultiInclusions<3>();
  multiInclusions.setInclusions(sphIncl);
  auto grid = vox::Voxellation<3>(multiInclusions);
  grid.setPureCoeffs({1.0, 3.0});
  grid.setHomogRule(homogenization::Rule::Voigt);
  grid.setVoxelRule(vox::VoxelRule::Average);
  grid.proceed({32, 32, 32});
  //   grid.printFile("Zones.vtk", "Coeffs.txt");

  auto homogenized_matrix = amx::computeThermalCoeff(grid);
  for (auto row : homogenized_matrix) {
    std::cout << row[0] << "\t" << row[1] << "\t" << row[2] << "\n";
  }
}