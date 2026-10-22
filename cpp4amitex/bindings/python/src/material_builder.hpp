/*
SPDX-License-Identifier: LGPL-3.0-only

(C) copyright 2026 of CEA

author: Guillaume Bareigts <guillaume.bareigts@cea.fr>, CEA
*/
#ifndef _AMITEX_PYTHON_MATERIAL_BUILDER_HEADER_
#define _AMITEX_PYTHON_MATERIAL_BUILDER_HEADER_

#include <pybind11/pybind11.h>

namespace amitex_python {

void defineMaterialBuilderMod(pybind11::module_& m);

}

#endif  // _AMITEX_PYTHON_INPUT_HEADER_
