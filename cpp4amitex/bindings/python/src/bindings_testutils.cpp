/*
SPDX-License-Identifier: LGPL-3.0-only

(C) copyright 2026 of CEA

author: Guillaume Bareigts <guillaume.bareigts@cea.fr>, CEA
*/
#include <pybind11/pybind11.h>

#include "testutils.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

using namespace amitex_tests;

PYBIND11_MODULE(testutils, m) {
  m.def("compareVtkWithRef", &compareVtkWithRef);
  m.def("compareBinWithRef", &compareBinWithRef);
}