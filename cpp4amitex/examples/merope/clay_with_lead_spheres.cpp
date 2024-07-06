#include <iostream>

#include "MultiInclusions/MultiInclusions.hxx"
#include "MultiInclusions/SphereInclusions.hxx"
#include "Voxellation/Voxellation.hxx"

#include "amitex_compute.hpp"

using namespace merope;
using namespace sac_de_billes;

int main() {
  constexpr double mm = 1.0e-3;
  constexpr double r0 = 1 * mm;
  constexpr double r1 = 2 * mm;
  constexpr double r2 = 3.5 * mm;
  constexpr double l = 0.25 * mm;
  constexpr double phi1 = 0.2;
  constexpr double phi2 = 0.3;
  constexpr double lambda0_clay = 0.6;
  constexpr double lambda_lead = 35.3;
  constexpr double lambda_silver = 429;
  constexpr double L = 15 * mm;

  auto sphIncl = SphereInclusions<3>();
  sphIncl.setLength({L, L, L});
  sphIncl.fromHisto(0, algoSpheres::TypeAlgo::WP, 0.0, {{r1, phi1}, {r2, phi2}}, {1, 1});

  auto multiInclusions = MultiInclusions<3>();
  multiInclusions.setInclusions(sphIncl);
  multiInclusions.addLayer(multiInclusions.getAllIdentifiers(), 2, l);
  // TODO: maybe add field
  auto grid = vox::Voxellation<3>(multiInclusions);
  grid.setPureCoeffs({lambda0_clay, lambda_lead, lambda_silver});
  grid.setHomogRule(homogenization::Rule::Reuss);
  grid.setVoxelRule(vox::VoxelRule::Average);
  grid.proceed({32, 32, 32});
  // grid.printFile("Zones.vtk", "Coeffs.txt");
  // grid.printFieldFile("Coeffs.vtk");

  auto homogenized_matrix = amx::computeThermalCoeff(grid);
  for (auto row : homogenized_matrix) {
    std::cout << row[0] << "\t" << row[1] << "\t" << row[2] << "\n";
  }
}
