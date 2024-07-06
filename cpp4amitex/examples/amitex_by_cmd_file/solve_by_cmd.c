#include <string.h>

#include <mpi.h>
#include "amitex.h"

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  int fcomm = MPI_Comm_c2f(MPI_COMM_WORLD);

  int sim = amitex_simulation_new();
  amitex_set_mpi_comm(fcomm);
  const char cmds[] = "commands.in";
  amitex_initialize_from_cmd_file(sim, cmds, strlen(cmds));
  amitex_solve_all_loadings(sim);
  amitex_finalize(sim);
  amitex_simulation_delete(sim);

  MPI_Finalize();
}