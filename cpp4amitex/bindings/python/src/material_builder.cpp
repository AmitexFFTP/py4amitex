#include "material_builder.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "amitex/input/material_builder.hpp"

namespace py = pybind11;
using namespace amitex;
using namespace pybind11::literals;

namespace amitex_python {

void defineMaterialBuilderMod(pybind11::module_& m) {
  py::class_<VoxelSpec>(m, "VoxelSpec")
      .def(py::init<>())
      .def(py::init<const std::vector<std::tuple<size_t, double>>&>(), "phases"_a)
      .def(py::init<const std::vector<std::tuple<size_t, double, size_t>>&>(), "phases"_a)
      .def_readwrite("phases", &VoxelSpec::phases);

  py::class_<MaterialBuilder>(m, "MaterialBuilder")
      .def(py::init<>())
      .def(py::init<double>(), "minVolFrac"_a)
      .def("addVoxel",
           static_cast<void (MaterialBuilder::*)(Materials&, GridLinPoint, const VoxelSpec&,
                                                 Vector3D)>(&MaterialBuilder::addVoxel),
           "materials"_a, "position"_a, "spec"_a, "normal"_a = Vector3D{1.0, 0.0, 0.0})
      .def("addVoxel",
           static_cast<void (MaterialBuilder::*)(Materials&, GridLinPoint, size_t, size_t)>(
               &MaterialBuilder::addVoxel),
           "materials"_a, "position"_a, "numMaterial"_a, "zone"_a = 0);

  // void buildMaterials(Materials& materials, GridSize dims, const std::vector<ShortSpec>& phases,
  //                     IndexOrdering ordering = IndexOrdering::C);
  py::enum_<IndexOrdering>(m, "IndexOrdering")
      .value("C", IndexOrdering::C)
      .value("Fortran", IndexOrdering::Fortran);

  m.def("buildMaterials",
        static_cast<void (*)(Materials&, GridSize, const std::vector<VoxelSpec>&, IndexOrdering)>(
            &buildMaterials),
        "materials"_a, "dims"_a, "phases"_a, "ordering"_a = IndexOrdering::C);

  m.def("buildMaterials",
        static_cast<void (*)(Materials&, GridSize,
                             const std::vector<std::tuple<VoxelSpec, Vector3D>>&, IndexOrdering)>(
            &buildMaterials),
        "materials"_a, "dims"_a, "phases"_a, "ordering"_a = IndexOrdering::C);

  m.def("buildMaterials",
        static_cast<void (*)(Materials&, GridSize, const std::vector<ShortSpec>&, IndexOrdering)>(
            &buildMaterials),
        "materials"_a, "dims"_a, "phases"_a, "ordering"_a = IndexOrdering::C);

  m.def("buildMaterials",
        static_cast<void (*)(Materials&, GridSize,
                             const std::vector<std::tuple<ShortSpec, Vector3D>>&, IndexOrdering)>(
            &buildMaterials),
        "materials"_a, "dims"_a, "phases"_a, "ordering"_a = IndexOrdering::C);

  m.def("buildMaterialsFromVtk", &buildMaterialsFromVtk, "materials"_a, "materialIdPath"_a = "",
        "zoneIdPath"_a = "", "minId"_a = 1);
}

}  // namespace amitex_python