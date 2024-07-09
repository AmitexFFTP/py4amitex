#include <gtest/gtest.h>

#include "amitex/input.hpp"
#include "amitex/input/interphase.hpp"
#include "amitex/input/material_builder.hpp"
#include "amitex/input/materials.hpp"

#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

TEST(Composite, InterphaseDef) {
  Interphase inter;
  inter.addMaterial(0, 3);
  inter.addMaterial(2, 4);
  inter.addZones(1, {2, 4, 6});
  EXPECT_STREQ(
      toXMLString(inter).c_str(),
      R"(<Interphase><Interphase_material numM="1" Nzones="3"/><Interphase_material numM="3" Nzones="4"/><Interphase_zone_list numM="2" Nzones="3"><ZoneList>2 4 6 </ZoneList></Interphase_zone_list></Interphase>)");
}

TEST(Composite, InterphaseFromMat) {
  // |0 1 0 | -> |C C C|
  // |0 0 0 |    |0 0 C|

  Grid grid{{3, 2, 1}, {1., 1., 1.}};

  Material mat0, mat1;
  mat0.setLaw("elasiso");
  mat0.setCoeffComposites({1., 2.});
  mat1.setLaw("elasiso");
  mat1.setCoeffComposites({3., 4.});

  Zone zone0_1(grid.dims(), {{0, 0, 0}, {1, 0, 0}});
  Zone zone0_2(grid.dims(), {{0, 1, 0}});
  Zone zone1_0(grid.dims(), {{1, 1, 0}, {1, 1, 0}});
  Zone zone0_0(grid.dims(), {{2, 0, 0}, {2, 1, 0}});

  mat0.addZone(zone0_0);
  mat0.addZone(zone0_1);
  mat0.addZone(zone0_2);
  mat1.addZone(zone1_0);

  Composite composite{{0, 1}, "voigt"};
  composite.addVoxel(grid.linearize({0, 1, 0}), {0.1, 0.9});
  composite.addVoxel(grid.linearize({1, 1, 0}), {0.7, 0.3});
  composite.addVoxel(grid.linearize({2, 0, 0}), {0.6, 0.4});
  composite.addVoxel(grid.linearize({2, 1, 0}), {0.5, 0.5});

  Materials materials;
  materials.add(std::move(mat0));
  materials.add(std::move(mat1));
  materials.composites.add(std::move(composite));

  Input input{grid, AlgorithmParameters{Algorithm{"Basic_Scheme", true}}, std::move(materials),
              LoadingOutput{}};

  input.resultsDir = "testresults/interphasefrommat";
  input.generateFiles();
  ASSERT_TRUE(input.materials.interphase.has_value());
  EXPECT_STREQ(
      toXMLString(input.materials.interphase.value()).c_str(),
      R"(<Interphase><Interphase_material numM="2" Nzones="1"/><Interphase_zone_list numM="1" Nzones="2"><ZoneList>1 3 </ZoneList></Interphase_zone_list></Interphase>)");
}

TEST(Composite, BugTangentForDefaultNormals) {
  Grid grid{{3, 2, 1}, {1., 1., 1.}};

  Material mat;
  mat.setLaw("elasiso");
  mat.setCoeffComposites({1., 2.});

  std::vector<ShortSpec> voxels;
  for (auto lpos : grid.allLinPoints()) {
    voxels.push_back({{0, 0.5}, {0, 0.5}});
  }
  Materials materials;
  buildMaterials(materials, grid.dims(), voxels, IndexOrdering::Fortran);

  for (size_t i = 0; i < materials.composite(0).tangents().at(0).size(); i++) {
    for (size_t j = 0; j < materials.composite(0).tangents().at(0).at(i).size(); j++) {
      Vector3D t, n;
      for (size_t k = 0; k < 3; k++) {
        t[k] = materials.composite(0).tangents().at(k).at(i).at(j);
        n[k] = materials.composite(0).normals().at(k).at(i).at(j);
      }
      double prod2 = 0.0;
      for (size_t k = 0; k < 3; k++) prod2 += n[k] * t[k];
      EXPECT_EQ(prod2, prod2);  // crude Nan test
      EXPECT_NEAR(prod2, 0.0, 1.0e-7);
    }
  }
}

TEST(Composite, InterphaseFromMat2) {
  // |0 1 0 | -> |C 1 C|
  // |0 0 0 |    |C C C|

  Grid grid{{3, 2, 1}, {1., 1., 1.}};

  Material mat0, mat1, mat2;
  mat0.setLaw("elasiso");
  mat0.setCoeffComposites({1., 2.});
  mat1.setLaw("elasiso");
  mat1.setCoeffComposites({3., 4.});

  Materials materials;
  materials.add(std::move(mat0));
  materials.add(std::move(mat1));

  auto mat1pos = grid.linearize({0, 1, 0});
  auto mat2pos = grid.linearize({2, 0, 0});

  MaterialBuilder bd;
  for (auto p : grid.allPoints()) {
    auto lpos = grid.linearize(p);
    VoxelSpec spec;
    if (lpos == mat1pos) {
      spec = VoxelSpec({{0, 0.1}, {0, 0.2}, {1, 0.8}});
    } else if (lpos == mat2pos) {
      spec = VoxelSpec({{0, 1.}});
    } else {
      spec = VoxelSpec({{0, 0.1}, {1, 0.9}});
    }
    bd.addVoxel(materials, lpos, spec);
  }
  materials.composite(0).setLaw("laminate");

  Input input{grid, AlgorithmParameters{Algorithm{"Basic_Scheme", true}}, std::move(materials),
              LoadingOutput{}};

  input.resultsDir = "testresults/interphasefrommat2";
  input.generateFiles();
  ASSERT_TRUE(input.materials.interphase.has_value());
  EXPECT_STREQ(toXMLString(input.materials.interphase.value()).c_str(),
               R"(<Interphase><Interphase_material numM="2" Nzones="1"/></Interphase>)");
}
