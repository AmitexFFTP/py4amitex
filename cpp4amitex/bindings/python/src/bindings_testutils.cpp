#include <pybind11/pybind11.h>

#include "testutils.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

using namespace amitex_tests;

PYBIND11_MODULE(testutils, m) {
  m.def("compareVtkWithRef", &compareVtkWithRef);
  m.def("compareBinWithRef", &compareBinWithRef);
}