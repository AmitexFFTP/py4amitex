#include <mpi.h>

#include "amitex/component.hpp"

#include "pmvox.hpp"

int main(int argc, char* argv[]) {
  int rank;
  MPI_Init(&argc, &argv);
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  // Instantiation
  amitex::Component p;
  // Define MPI communicator
  p.setMPIComm(comm);
  // Initialize computation ()
  p.initialize("commands.in");

  for (int step = 1; step <= 4; step++) {
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
  // End computation
  p.terminate();

  MPI_Finalize();
}
