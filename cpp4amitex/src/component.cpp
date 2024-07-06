#include "amitex/component.hpp"

#include "amitex.h"
#include "amitex/errors.hpp"

namespace amitex {

Component::Component() {
  instance = amitex_simulation_new();
  if (instance < 0) throw std::runtime_error("amitex::Component: cannot create new instance");
}

Component::~Component() {
  ;
  amitex_simulation_delete(instance);
}

void Component::initialize(Input& input) {
  int rank = 0;
  MPI_Comm_rank(comm, &rank);
  const std::string& dir = input.resultsDir;

  if (rank == 0) {
    input.generateFiles();
  }
  MPI_Barrier(comm);
  std::string cmds = input.resultsDir + "/commands.in";
  amitex_initialize_from_cmd_file(instance, cmds.data(), cmds.size());
  outputPrefix_ = input.outputPrefix();
}

void Component::initialize(Input&& input) { initialize(input); }

void Component::initialize() { initialize(cmds); }

void Component::initialize(std::string_view cmdFile) {
  if (!cmdFile.empty()) {
    amitex_initialize_from_cmd_file(instance, cmdFile.data(), cmdFile.size());
  }
}

void Component::terminate() { amitex_finalize(instance); }
void Component::setMPIComm(MPI_Comm comm) {
  amitex_set_mpi_comm(MPI_Comm_c2f(comm));
  this->comm = comm;
}
void Component::initTimeStep(double dt) { amitex_init_timestep(instance, dt); }
std::pair<double, bool> Component::computeTimeStep() {
  bool stop;
  double dt = amitex_compute_timestep(instance, &stop);
  return {dt, stop};
}
bool Component::solveTimeStep() { return amitex_solve_timestep(instance); }
void Component::abortTimeStep() { amitex_abort_timestep(instance); }
void Component::validateTimeStep() { amitex_validate_timestep(instance); }
void Component::solveAllLoadings() { amitex_solve_all_loadings(instance); }
void Component::setLoading(int index_VarD, int component, double value, DiffusionDriving driving) {
  throw AmitexError{"setLoading NOT IMPLEMENTED"};
}
void Component::setLoading(Component component, double value, MechanicDriving driving) {
  throw AmitexError{"setLoading NOT IMPLEMENTED"};
}

std::string Component::outputPrefix() { return outputPrefix_; }

}  // namespace amitex