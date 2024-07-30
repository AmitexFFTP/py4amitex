#include <gtest/gtest.h>

#include "amitex/input/field.hpp"

#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

TEST(Fields, FieldInt) {
  Field<int> field({5, 6, 2});
  field(0, 3, 1) = 4;
  EXPECT_EQ(field.at(0, 3, 1), 4);
  EXPECT_EQ(field.lbound(0), 0);
  EXPECT_EQ(field.lbound(1), 0);
  EXPECT_EQ(field.lbound(2), 0);
  EXPECT_EQ(field.ubound(0), 5);
  EXPECT_EQ(field.ubound(1), 6);
  EXPECT_EQ(field.ubound(2), 2);
  EXPECT_EQ(field.dims(), (std::array<size_t, 3>{5, 6, 2}));

  EXPECT_TRUE(field.inBounds({1, 2, 0}));
  EXPECT_TRUE(field.inBounds({4, 5, 1}));
  EXPECT_TRUE(field.inBounds({2, 4, 0}));
  EXPECT_TRUE(field.inBounds({2, 0, 1}));
  EXPECT_FALSE(field.inBounds({2, static_cast<size_t>(-1), 1}));
  EXPECT_FALSE(field.inBounds({0, 0, 2}));
  EXPECT_FALSE(field.inBounds({6, 5, 1}));
  EXPECT_FALSE(field.inBounds({4, 6, 0}));

  field.fill(6);
  for (size_t i = 0; i < 5; i++) {
    for (size_t j = 0; j < 6; j++) {
      for (size_t k = 0; k < 2; k++) {
        EXPECT_EQ(field.at(i, j, k), 6);
      }
    }
  }

  EXPECT_THROW(field.at(0, 3, 2), std::out_of_range);

  Field view = field;

  view[{0, 4, 1}] = -61;
  EXPECT_EQ(field(0, 4, 1), -61);
  view[{4, 5, 1}] = -62;
  EXPECT_EQ(field(4, 5, 1), -62);

  Field newf = field.copy();
  std::array<GridPoint, 3> points = {GridPoint{0, 4, 1}, {4, 5, 1}, {1, 0, 1}};
  for (auto p : points) {
    int expected = field[p];
    newf[p] = -33;
    EXPECT_EQ(field[p], expected);
  }
}