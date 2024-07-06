#include <mpi.h>

#include "amitex/component.hpp"
#include "amitex/extract.hpp"

#include "pmvox.hpp"

void printAverages(amitex::Component& p);

int main(int argc, char* argv[]) {
  int rank;
  MPI_Init(&argc, &argv);
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  amitex::Input input = make_thermo_pmvox_input();

  // Instantiation
  amitex::Component p;
  // Define MPI communicator
  p.setMPIComm(comm);
  // Initialize computation ()
  p.initialize(input);

  for (int step = 1; step <= 1; step++) {
    // Define next timestep
    auto [dt, _] = p.computeTimeStep();
    p.initTimeStep(dt);
    // Compute timestep
    bool converged = p.solveTimeStep();
    if (!converged) {
      p.abortTimeStep();
      std::cerr << "Note: step " << step << " has not converged" << std::endl;
      MPI_Abort(comm, 1);
    }
    // Validate timestep
    p.validateTimeStep();
  }
  if (rank == 0) printAverages(p);
  // End computation
  p.terminate();

  MPI_Finalize();
}

void printAverages(amitex::Component& p) {
  amitex::Extract ext{p.outputPrefix()};
  auto fluxD = ext.averageDiffusionFlux(0);
  std::cout << "Flux     = " << fluxD[0] << '\t' << fluxD[1] << '\t' << fluxD[2] << std::endl;
  auto gradD = ext.averageDiffusionGradient(0);
  std::cout << "Gradient = " << gradD[0] << '\t' << gradD[1] << '\t' << gradD[2] << std::endl;
}
