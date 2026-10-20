#include <iostream>

#include "amitex/extract.hpp"
#include "amitex/simulation.hpp"

#include "pmvox.hpp"

void printAverages(amitex::Input& p);

int main(int argc, char* argv[]) {
  auto input = make_thermo_pmvox_input_gen("amitex_results_2zones", 1);

  runSimulationExternal(*input);

  printAverages(*input);
}

void printAverages(amitex::Input& p) {
  amitex::Extract ext{p.outputPrefix()};
  auto fluxD = ext.averageDiffusionFlux(0);
  std::cerr << "Flux     = " << fluxD[0] << '\t' << fluxD[1] << '\t' << fluxD[2] << std::endl;
  auto gradD = ext.averageDiffusionGradient(0);
  std::cerr << "Gradient = " << gradD[0] << '\t' << gradD[1] << '\t' << gradD[2] << std::endl;
}
