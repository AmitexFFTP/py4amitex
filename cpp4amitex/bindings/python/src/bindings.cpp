#include <pybind11/pybind11.h>

#include "field.hpp"
#include "input_general.hpp"
#include "material_builder.hpp"
#include "simulation.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

using namespace amitex_python;

PYBIND11_MODULE(input, m) {
  defineInputMod(m);
  auto fieldMod = m.def_submodule("field");
  defineFieldMod(fieldMod);
  auto materialBuilderMod = m.def_submodule("materialbuilder");
  defineMaterialBuilderMod(materialBuilderMod);
  auto runextMod = m.def_submodule("simulation");
  defineSimulationMod(runextMod);
}