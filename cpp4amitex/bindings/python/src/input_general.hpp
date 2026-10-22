/*
SPDX-License-Identifier: LGPL-3.0-only

(C) copyright 2026 of CEA

author: Guillaume Bareigts <guillaume.bareigts@cea.fr>, CEA
*/
#ifndef _AMITEX_PYTHON_INPUT_HEADER_
#define _AMITEX_PYTHON_INPUT_HEADER_

#include <pybind11/pybind11.h>

namespace amitex_python {

void defineInputMod(pybind11::module_& m);

}

#endif  // _AMITEX_PYTHON_INPUT_HEADER_
