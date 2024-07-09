#include "simulation.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "amitex/simulation.hpp"

#include "docstrings.hpp"

namespace py = pybind11;
using namespace amitex;
using namespace pybind11::literals;

namespace amitex_python {

void defineSimulationMod(pybind11::module_& m) {
  m.def(
      "runSimulationExternal",
      [](Input& input, int numberProcs) { runSimulationExternal(input, numberProcs); }, "input"_a,
      "numberProcs"_a = 0, DOC(amitex, runSimulationExternal));
  m.def("getSimulationShellCommand", &getSimulationShellCommand, "input"_a, "numberProcs"_a = 0, DOC(amitex, getSimulationShellCommand));
}

}  // namespace amitex_python
