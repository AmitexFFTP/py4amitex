#include "amitex/simulation.hpp"

#include "amitex/component.hpp"

namespace amitex {

void runSimulation(Input &input, MPI_Comm comm) {
  Component p;
  p.setMPIComm(comm);
  p.initialize(input);
  p.solveAllLoadings();
  p.terminate();
}

}  // namespace amitex
