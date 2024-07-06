#include <gtest/gtest.h>

#include "amitex/io.hpp"

#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

TEST(IO, VTK) {
  VtkHeader header;
  std::vector<int64_t> data0;
  header = readVTK("ref-amxdir/bilayer/zoneIds.vtk", data0);
  for (size_t k = 0; k < 3; k++) EXPECT_EQ(header.dimensions[k], 40);
  for (size_t k = 0; k < 3; k++) EXPECT_NEAR(header.spacing[k], 2.5e-2, 1.e-8);
  for (size_t k = 0; k < 3; k++) EXPECT_NEAR(header.origin[k], 0.0, 1.e-8);
  EXPECT_EQ(header.cellDataSize, 64000);
  EXPECT_STREQ(header.scalarType.c_str(), "long");
  EXPECT_EQ(data0.size(), 64000);

  std::vector<double> data1;
  header = readVTK("ref-amxdir/elaiso_eigs/intvar_1_1.vtk", data1);
  for (size_t k = 0; k < 3; k++) EXPECT_EQ(header.dimensions[k], 32);
  for (size_t k = 0; k < 3; k++) EXPECT_NEAR(header.spacing[k], 1.0, 1.e-8);
  for (size_t k = 0; k < 3; k++) EXPECT_NEAR(header.origin[k], 0.0, 1.e-8);
  EXPECT_EQ(header.cellDataSize, 32768);
  EXPECT_STREQ(header.scalarType.c_str(), "double");
  EXPECT_EQ(data1.size(), 32768);
}

TEST(IO, BIN) {
  std::vector<double> data0, data1;
  auto type = readBin("ref-amxdir/bilayer/Coeff1_1.bin", data0);
  EXPECT_EQ(data0.size(), 2);
  EXPECT_STREQ(type.c_str(), "double");

  type = readBin("ref-amxdir/simple_spheres_composites/composites/rep_1_2/pos.bin", data1);
  EXPECT_EQ(data1.size(), 6626);
  EXPECT_STREQ(type.c_str(), "unsigned_long");
}