#ifndef _AMITEX_PYTHON_FIELD_HEADER_
#define _AMITEX_PYTHON_FIELD_HEADER_

#include <pybind11/pybind11.h>

namespace amitex_python {

void defineFieldMod(pybind11::module_& m);

}  // namespace amitex_python

#endif  // _AMITEX_PYTHON_FIELD_HEADER_
