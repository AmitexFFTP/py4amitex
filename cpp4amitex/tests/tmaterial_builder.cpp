#include <gtest/gtest.h>

#include "amitex/input.hpp"
#include "amitex/input/material_builder.hpp"

#include "amitex/io.hpp"

#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

TEST(MaterialBuilder, FromVTKBilayer) {
  std::filesystem::path refDir = "ref-amxdir/bilayer";
  std::filesystem::path dir = "testresults/addzoneandlaunchbilayer";

  VtkHeader header;
  auto materials = Materials::create();
  auto grid = buildMaterialsFromVtk(*materials, refDir / "materialIds.vtk", refDir / "zoneIds.vtk");
  auto input = Input::create(grid, AlgorithmParameters::create(Algorithm::createDefault()),
                             materials, LoadingOutput::create());
  input->resultsDir = dir;

  for (size_t m = 0; m < 2; m++) {
    input->materials->material(m)->setLaw("elasiso");
    input->materials->material(m)->setNumberCoeff(2);
    for (size_t j = 0; j < 2; j++) {
      std::stringstream fname;
      fname << "Coeff" << m + 1 << '_' << j + 1 << ".bin";
      input->materials->material(m)->setCoeffZoneFromBin(j, refDir / fname.str());
    }
  }
  input->materials->referenceMaterial = ReferenceMaterial::create(250e6, 250e6);

  input->generateFiles();

  EXPECT_TRUE(compareXMLFiles(dir / "materials.xml", refDir / "materials.xml"));

  EXPECT_TRUE(compareVtkWithRef(dir / "materialIds.vtk", refDir / "materialIds.vtk", 0.0));
  EXPECT_TRUE(compareVtkWithRef(dir / "zoneIds.vtk", refDir / "zoneIds.vtk", 0.0));

  for (int i = 1; i <= 2; i++) {
    for (int j = 1; j < 2; j++) {
      std::stringstream fname;
      fname << "Coeff" << i << '_' << j << ".bin";
      EXPECT_TRUE(compareBinWithRef(dir / fname.str(), refDir / fname.str(), 0.0));
    }
  }
}

TEST(MaterialBuilder, FromVTKMat) {
  auto materials = Materials::create();
  auto grid = buildMaterialsFromVtk(*materials, "data/arlequin_N3_r1.vtk");
  EXPECT_EQ(materials->numberMaterials(), 27);
  for (int m = 0; m < 27; m++) {
    Material& mat = *materials->material(m);
    EXPECT_EQ(mat.numberZones(), 1);
    EXPECT_EQ(mat.zones().at(0)->numberVoxels(), 1);
    auto lpos = mat.zones().at(0)->linearPositions().at(0);
    EXPECT_EQ(lpos, m);
  }
}

TEST(MaterialBuilder, FromVTKZone) {
  auto materials = Materials::create();
  auto grid = buildMaterialsFromVtk(*materials, "", "data/arlequin_N3_r1.vtk");
  EXPECT_EQ(materials->numberMaterials(), 1);
  Material& mat = *materials->material(0);
  EXPECT_EQ(mat.numberZones(), 27);
  for (int z = 0; z < 27; z++) {
    EXPECT_EQ(mat.zones().at(z)->numberVoxels(), 1);
    auto lpos = mat.zones().at(z)->linearPositions().at(0);
    EXPECT_EQ(lpos, z);
  }
}