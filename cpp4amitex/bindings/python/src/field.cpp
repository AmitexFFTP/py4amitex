#include "field.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include "amitex/errors.hpp"
#include "amitex/input/field.hpp"

#include "docstrings.hpp"

namespace py = pybind11;
using namespace amitex;
using namespace pybind11::literals;

namespace amitex_python {

template <typename T>
static void defField(pybind11::module_& m, const char* name) {
  using FieldT = Field<T>;
  py::class_<FieldT>(m, name, py::buffer_protocol())
      .def(py::init<GridSize>())
      .def(py::init([](py::buffer b, bool copy) {
             py::buffer_info info = b.request();
             if (info.format != py::format_descriptor<T>::format())
               throw InputError(std::string{"Field: incompatible data type"});
             if (info.ndim != 3) throw InputError("Field needs 3 dimensions");
             size_t nx = info.shape[0];
             size_t ny = info.shape[1];
             size_t nz = info.shape[2];
             constexpr py::ssize_t Tsize = sizeof(T);
             // Fortran-contiguous, no copy necessary
             bool canNoCopy = info.strides[0] == Tsize && info.strides[1] == Tsize * nx &&
                              info.strides[2] == Tsize * nx * ny;
             if (copy) {
               Field<T> ret{{nx, ny, nz}};
               for (size_t ix = 0; ix < nx; ix++) {
                 for (size_t iy = 0; iy < ny; iy++) {
                   for (size_t iz = 0; iz < nz; iz++) {
                     ret[{ix, iy, iz}] =
                         *(static_cast<T*>(info.ptr) +
                           (info.strides[0] * ix + info.strides[1] * iy + info.strides[2] * iz) /
                               Tsize);
                   }
                 }
               }
               return ret;
             } else if (canNoCopy) {
               return Field{{nx, ny, nz}, static_cast<T*>(info.ptr)};
             } else {
               throw InputError{
                   "Cannot avoid to copy buffer into Field as the buffer needs to be "
                   "Fortran-contiguous"};
             }
           }),
           "buffer"_a, "copy"_a = false, "Init from an array-like type (eg Numpy array)")
      .def("dims", &FieldT::dims, DOC(amitex, Field, dims))
      .def_property_readonly("shape", &FieldT::dims)
      .def("fill", &FieldT::fill, DOC(amitex, Field, fill))
      .def("inBounds", static_cast<bool (FieldT::*)(GridPoint p)>(&FieldT::inBounds),
           DOC(amitex, Field, inBounds))
      .def("inBounds", static_cast<bool (FieldT::*)(size_t, size_t, size_t)>(&FieldT::inBounds),
           DOC(amitex, Field, inBounds, 2))
      .def("lbound", &FieldT::lbound, DOC(amitex, Field, lbound))
      .def("ubound", &FieldT::ubound, DOC(amitex, Field, ubound))
      .def("at", static_cast<T& (FieldT::*)(size_t, size_t, size_t)>(&FieldT::at),
           DOC(amitex, Field, at))
      .def_buffer([](FieldT& fd) -> py::buffer_info {
        auto dims = fd.dims();
        return py::buffer_info(fd.data(),                          /* Pointer to buffer */
                               sizeof(T),                          /* Size of one scalar */
                               py::format_descriptor<T>::format(), /* Python format descriptor */
                               3,                                  /* Number of dimensions */
                               {dims[0], dims[1], dims[2]},        /* Buffer dimensions */
                               {sizeof(T), /* Strides (in bytes) for each index */
                                sizeof(T) * dims[0], sizeof(T) * dims[0] * dims[1]});
      })
      .def("__len__",
           [](FieldT& fd) {
             auto dims = fd.dims();
             return dims[0] * dims[0] * dims[0];
           })
      .def("__getitem__",
           [](FieldT& fd, std::array<size_t, 3> i) { return fd.at(i[0], i[1], i[2]); })
      .def("__setitem__",
           [](FieldT& fd, std::array<size_t, 3> i, T v) { fd.at(i[0], i[1], i[2]) = v; })
      .def_static("loadFromVtk", &FieldT::loadFromVtk, DOC(amitex, Field, loadFromVtk));
}

void defineFieldMod(pybind11::module_& m) {
  defField<double>(m, "FieldDouble");
  defField<int>(m, "FieldInt");
}

}  // namespace amitex_python
