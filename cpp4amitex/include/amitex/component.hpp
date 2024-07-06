#ifndef _AMITEX_COMPONENT_HEADER_
#define _AMITEX_COMPONENT_HEADER_
#include "amitex/config.hpp"
#ifndef AMITEX_DRIVER_API
#error "AMITEX driver API is not compiled"
#endif

#include <mpi.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "amitex/input.hpp"

namespace amitex {

class Component {
 public:
  Component();
  ~Component();
  void initialize();
  void initialize(std::string_view cmdFile);
  void initialize(Input& input);
  void initialize(Input&& input);
  void terminate();
  void setMPIComm(MPI_Comm comm);
  void initTimeStep(double dt);
  std::pair<double, bool> computeTimeStep();
  bool solveTimeStep();
  void abortTimeStep();
  void validateTimeStep();
  void solveAllLoadings();
  void setLoading(int index_VarD, int component, double value, DiffusionDriving driving);
  void setLoading(Component component, double value, MechanicDriving driving);
  std::string outputPrefix();

 private:
  int instance;
  std::string cmds = "commands.in";
  MPI_Comm comm = MPI_COMM_WORLD;
  std::string outputPrefix_;
};

}  // namespace amitex

#endif  // _AMITEX_COMPONENT_HEADER_