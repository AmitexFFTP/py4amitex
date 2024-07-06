#ifndef _AMITEX_MATERIAL_BUILDER_
#define _AMITEX_MATERIAL_BUILDER_

#include <map>
#include <utility>
#include <vector>

#include "amitex/input/grid.hpp"
#include "amitex/input/materials.hpp"

//! \file material_builder.hpp
//! \brief Higher-level contruction of materials from voxel specification
//!
//! \note This module is concerned with construction of zones and composite specification, the user
//! must specify the material behavior (law, coefficient)
//!
//! \example
//! ```C++
//! #include "amitex.hpp"
//!
//! int main() {
//! #include "amitex/input.hpp"
//! #include "amitex/input/material_builder.hpp"
//! int main() {
//!   using namespace amitex;
//!   Materials materials;
//!   Material mat0, mat1;
//!   mat0.setLaw("elasiso");
//!   mat0.setCoeffs({1., 2.});
//!   mat1.setLaw("elasiso");
//!   mat1.setCoeffs({3., 4.});
//!   materials.add(mat0);
//!   materials.add(mat1);
//!   Grid grid{{2, 2, 2}, {1., 1., 1.}};
//!   MaterialBuilder matBuilder;
//!   // Not composite voxels
//!   for (GridPoint p : grid.allPoints()) {
//!     if (p[0] != 0 && p[1] != 0 && p[2] != 0) {
//!       matBuilder.addVoxel(materials, grid.linearize(p), 0);
//!     }
//!   }
//!   // 1 composite voxel: 30% material 0, 70%1, normal at the interface {0.0, 1.0, 0.0}
//!   matBuilder.addVoxel(materials, grid.linearize({0, 0, 0}), VoxelSpec{{{0, 0.3}, {1, 0.7}}},
//!                       {0.0, 1.0, 0.0});
//!   // 1 composite (0, 1) was created, we need to set its law
//!   materials.composite(0).setLaw("laminate");
//! }
//! ```

namespace amitex {

//! Basic specification (index, volume fraction, zone) of a voxel (composite if more than one voxel)
class VoxelSpec {
 public:
  VoxelSpec() = default;
  VoxelSpec(const std::vector<std::tuple<size_t, double>>& phases) {
    for (auto [pid, vf] : phases) this->phases.push_back({pid, vf, 0});
  }
  VoxelSpec(const std::vector<std::tuple<size_t, double, size_t>>& phases) : phases{phases} {}
  std::vector<std::tuple<size_t, double, size_t>>
      phases;  //! (material index, volume fraction, zone)
};

class MaterialBuilder {
 public:
  MaterialBuilder() = default;
  //! \param minVolFrac minimum volume fraction for a phase to be included
  //! (for more ion this problem see AMITEX doc for composites)
  MaterialBuilder(double minVolFrac) : minVolFrac{minVolFrac} {}

  //! Add a voxel with phases of  (num_material, volume phraction)
  //! \param materials where the voxel will be added
  //! \param pos linear position
  //! \param spec voxel specification
  //! \param normal normal of the interface
  void addVoxel(Materials& materials, GridLinPoint pos, const VoxelSpec& spec,
                Vector3D normal = {1.0, 0.0, 0.0});

  //! Add a non-composite voxel
  //! \param numM index of material
  void addVoxel(Materials& materials, GridLinPoint pos, size_t numM, size_t zone = 0) {
    this->addVoxel(materials, pos, VoxelSpec{{{numM, 1.0, zone}}});
  }

 private:
  double minVolFrac = Composite::maxVolumeFraction;
  std::map<std::vector<size_t>, size_t> compos;

  void addVoxelAux(Materials& materials, GridLinPoint pos, const std::vector<size_t>& phases,
                   const std::vector<double>& volfracs, const std::vector<size_t>& zone,
                   Vector3D normal);
};

//! Order or elements in memory for multidimentional arrays represented by vector<>
//! or, alternatively, order of linearized positions
enum class IndexOrdering { C, Fortran };

//! Build materials from a list of all voxels (ie indexed by their linearized positions)
//! \param phases array as field of voxel specs
//! \param ordering ordering of positions
void buildMaterials(Materials& materials, GridSize dims, const std::vector<VoxelSpec>& phases,
                    IndexOrdering ordering = IndexOrdering::C);

//! Build materials from a list of all voxels (ie indexed by their linearized positions)
//! with normal information
//! \param phases array as tuple (voxel specs, normal)
//! \param ordering ordering of positions
void buildMaterials(Materials& materials, GridSize dims,
                    const std::vector<std::tuple<VoxelSpec, Vector3D>>& phases,
                    IndexOrdering ordering = IndexOrdering::C);

//! For Mérope interop
using ShortSpec = std::vector<std::tuple<unsigned short, double>>;

void buildMaterials(Materials& materials, GridSize dims, const std::vector<ShortSpec>& phases,
                    IndexOrdering ordering = IndexOrdering::C);

//! With normals
void buildMaterials(Materials& materials, GridSize dims,
                    const std::vector<std::tuple<ShortSpec, Vector3D>>& phases,
                    IndexOrdering ordering = IndexOrdering::C);

//! Build materials from VTK files 'material ids' and 'zone ids'
//! \param materials
//! \param materialIdPath path to VTK file 'material ids'
//! \param zoneIdPath path to VTK file 'zone ids'
//! \param minId minimum id (typically for AMITEX it is 1)
Grid buildMaterialsFromVtk(Materials& materials, const std::string& materialIdPath = std::string{},
                           const std::string& zoneIdPath = std::string{}, int minId = 1);

}  // namespace amitex

#endif  // _AMITEX_MATERIAL_BUILDER_