#include "simulation.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>

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
  m.def("getSimulationShellCommand", &getSimulationShellCommand, "input"_a, "numberProcs"_a = 0,
        DOC(amitex, getSimulationShellCommand));
  m.def("runSimulationFromFiles", &runSimulationFromFiles, "algorithmPath"_a, "materialsPath"_a,
        "loadingPath"_a, "materialIdsPath"_a, "zoneIdsPath"_a, "outputPrefix"_a, "grid"_a,
        "numberProcs"_a = 0, DOC(amitex, runSimulationFromFiles));
  m.def("getSimulationShellCommandFromFiles", &getSimulationShellCommandFromFiles,
        "algorithmPath"_a, "materialsPath"_a, "loadingPath"_a, "materialIdsPath"_a, "zoneIdsPath"_a,
        "outputPrefix"_a, "grid"_a, "numberProcs"_a = 0, DOC(amitex, runSimulationFromFiles));
}

}  // namespace amitex_python
