#include <mpi.h>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "amitex/component.hpp"
#include "pmvox.hpp"

using amitex::Component;
using amitex::DiffusionDriving;

MPI_Comm setup_comm() {
  MPI_Comm all = MPI_COMM_WORLD;
  int rank;
  int nranks;
  MPI_Comm_rank(all, &rank);
  MPI_Comm_size(all, &nranks);
  if (nranks <= 1) return all;
  MPI_Group old_group, new_group;
  MPI_Comm_group(all, &old_group);
  int range[][3] = {{1, nranks - 1, 1}};
  MPI_Group_range_incl(old_group, 1, range, &new_group);
  MPI_Comm new_comm;
  MPI_Comm_create(all, new_group, &new_comm);
  return new_comm;
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  MPI_Comm ami_comm = setup_comm();
  if (ami_comm != MPI_COMM_NULL) {
    amitex::Component p;
    p.setMPIComm(ami_comm);
    p.initialize(make_thermo_pmvox_input("amitex_results_mpi0", ami_comm));

    bool stop = false;
    double dt;

    p.setLoading(0, Component::X, 0.0, DiffusionDriving::Gradient);
    p.setLoading(0, Component::Y, 0.0, DiffusionDriving::Gradient);
    p.setLoading(0, Component::Z, 0.0, DiffusionDriving::Gradient);

    for (int i = 1; i <= 3; i++) {
      p.setLoading(0, Component::X, 0.05 * i * i, DiffusionDriving::Gradient);
      auto [dt, stop] = p.computeTimeStep();
      // if (stop) break;
      p.initTimeStep(dt);
      int nl = 0;
      if (!p.solveTimeStep()) {
        p.abortTimeStep();
        std::cout << "Note: step " << i << " has not converged" << std::endl;
        abort();
      }
      p.validateTimeStep();
    }
    p.terminate();
  } else {
    std::cout << "Sleeping…\n";
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3000ms);
    std::cout << "Waking up…\n";
  }
  MPI_Finalize();
}
