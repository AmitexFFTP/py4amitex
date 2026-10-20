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

  Ptr<Material> mat0 = Material::create(), mat1 = Material::create();
  mat0->setLaw("elasiso");
  mat0->setCoeffComposites({1., 2.});
  mat1->setLaw("elasiso");
  mat1->setCoeffComposites({3., 4.});

  auto zone0_1 = Zone::create(grid.dims(), {{0, 0, 0}, {1, 0, 0}});
  auto zone0_2 = Zone::create(grid.dims(), {{0, 1, 0}});
  auto zone1_0 = Zone::create(grid.dims(), {{1, 1, 0}, {1, 1, 0}});
  auto zone0_0 = Zone::create(grid.dims(), {{2, 0, 0}, {2, 1, 0}});

  mat0->addZone(zone0_0);
  mat0->addZone(zone0_1);
  mat0->addZone(zone0_2);
  mat1->addZone(zone1_0);

  auto composite = Composite::create({0, 1}, "voigt");
  composite->addVoxel(grid.linearize({0, 1, 0}), {0.1, 0.9});
  composite->addVoxel(grid.linearize({1, 1, 0}), {0.7, 0.3});
  composite->addVoxel(grid.linearize({2, 0, 0}), {0.6, 0.4});
  composite->addVoxel(grid.linearize({2, 1, 0}), {0.5, 0.5});

  auto materials = Materials::create();
  materials->add(mat0);
  materials->add(mat1);
  materials->composites.add(std::move(composite));

  auto input =
      Input::create(grid, AlgorithmParameters::create(Algorithm::create("Basic_Scheme", true)),
                    std::move(materials), LoadingOutput::create());

  input->resultsDir = "testresults/interphasefrommat";
  input->generateFiles();
  ASSERT_TRUE(input->materials->interphase.has_value());
  EXPECT_STREQ(
      toXMLString(input->materials->interphase.value()).c_str(),
      R"(<Interphase><Interphase_material numM="2" Nzones="1"/><Interphase_zone_list numM="1" Nzones="2"><ZoneList>1 3 </ZoneList></Interphase_zone_list></Interphase>)");
}

TEST(Composite, BugTangentForDefaultNormals) {
  Grid grid{{3, 2, 1}, {1., 1., 1.}};

  auto mat = Material::create();
  mat->setLaw("elasiso");
  mat->setCoeffComposites({1., 2.});

  std::vector<ShortSpec> voxels;
  for (auto lpos : grid.allLinPoints()) {
    voxels.push_back({{0, 0.5}, {0, 0.5}});
  }
  auto materials = Materials::create();
  buildMaterials(*materials, grid.dims(), voxels, IndexOrdering::Fortran);

  for (size_t i = 0; i < materials->composite(0)->tangents().at(0).size(); i++) {
    for (size_t j = 0; j < materials->composite(0)->tangents().at(0).at(i).size(); j++) {
      Vector3D t, n;
      for (size_t k = 0; k < 3; k++) {
        t[k] = materials->composite(0)->tangents().at(k).at(i).at(j);
        n[k] = materials->composite(0)->normals().at(k).at(i).at(j);
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

  auto mat0 = Material::create();
  auto mat1 = Material::create();
  auto mat2 = Material::create();
  mat0->setLaw("elasiso");
  mat0->setCoeffComposites({1., 2.});
  mat1->setLaw("elasiso");
  mat1->setCoeffComposites({3., 4.});

  auto materials = Materials::create();
  materials->add(std::move(mat0));
  materials->add(std::move(mat1));

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
    bd.addVoxel(*materials, lpos, spec);
  }
  materials->composite(0)->setLaw("laminate");

  auto input =
      Input::create(grid, AlgorithmParameters::create(Algorithm::create("Basic_Scheme", true)),
                    materials, LoadingOutput::create());

  input->resultsDir = "testresults/interphasefrommat2";
  input->generateFiles();
  ASSERT_TRUE(input->materials->interphase.has_value());
  EXPECT_STREQ(toXMLString(input->materials->interphase.value()).c_str(),
               R"(<Interphase><Interphase_material numM="2" Nzones="1"/></Interphase>)");
}

TEST(Composite, ZonesPerPhase) {
  Grid grid{{2, 1, 1}, {1., 1., 1.}};

  auto mat0 = Material::create();
  mat0->setLaw("elasiso");
  mat0->setNumberCoeffComposite(2);
  mat0->setCoeffCompositeZone(0, {1., 2.});
  mat0->setCoeffCompositeZone(1, {1.1, 2.2});

  auto materials = Materials::create();
  materials->add(std::move(mat0));

  auto mat1pos = grid.linearize({0, 1, 0});
  auto mat2pos = grid.linearize({2, 0, 0});

  MaterialBuilder bd;

  for (auto p : grid.allPoints()) {
    auto lpos = grid.linearize(p);
    VoxelSpec spec;
    if (lpos == 0) {
      spec = VoxelSpec({{0, 0.1, 0}, {0, 0.9, 1}});
    } else
      spec = VoxelSpec({{0, 1.}});
    bd.addVoxel(*materials, lpos, spec);
  }
  materials->composite(0)->setLaw("reuss");

  auto input =
      Input::create(grid, AlgorithmParameters::create(Algorithm::create("Basic_Scheme", true)),
                    std::move(materials), LoadingOutput::create());

  input->resultsDir = "testresults/zonesperphase";
  input->generateFiles();
  std::vector<long long> zones;
  for (int m = 0; m < 2; m++) {
    std::ostringstream oss;
    oss << "testresults/zonesperphase/composites/rep_1_1/zone" << (m + 1) << ".bin";
    readBin(oss.str(), zones);
    ASSERT_EQ(zones.size(), 1);
    EXPECT_EQ(zones[0], m + 1);
  }
}
