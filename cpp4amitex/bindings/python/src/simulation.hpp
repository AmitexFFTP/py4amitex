#ifndef _AMITEX_PYTHON_SIMULATION_HEADER_
#define _AMITEX_PYTHON_SIMULATION_HEADER_

#include <pybind11/pybind11.h>

namespace amitex_python {

void defineSimulationMod(pybind11::module_& m);

}  // namespace amitex_python

#endif  // _AMITEX_PYTHON_SIMULATION_HEADER_
