#include <gtest/gtest.h>

#include "amitex/input/grid.hpp"

#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

TEST(Grid, AllPoints) {
  Grid grid{{2, 1, 2}, {0.1, 0.1, 0.1}};
  GridPoint refs[4] = {{0, 0, 0}, {1, 0, 0}, {0, 0, 1}, {1, 0, 1}};
  size_t npoints = 0;
  auto it = grid.allPoints().begin();
  auto end = grid.allPoints().end();
  ASSERT_TRUE(it != end);
  for (GridPoint point : grid.allPoints()) {
    EXPECT_EQ(point, refs[npoints]);
    npoints++;
    if (npoints > 6) break;
  }
  EXPECT_EQ(npoints, 4);
}

TEST(Grid, distance) {
  Grid grid{{2, 2, 3}, {0.1, 0.1, 0.2}};
  double d0 = grid.distance(GridPoint{0, 0, 0}, GridPoint{0, 1, 0});
  EXPECT_NEAR(d0, 0.1, eps);
  double d1 = grid.distance(GridPoint{0, 0, 0}, GridPoint{1, 1, 0});
  EXPECT_NEAR(d1, 0.1 * sqrt(2), eps);
  double d2 = grid.distance(GridPoint{0, 0, 0}, GridPoint{1, 1, 1});
  EXPECT_NEAR(d2, 0.1 * sqrt(6), eps);
  double d3 = grid.distance(GridPoint{0, 0, 0}, Vector3D{0.1, 0.1, 0.2});
  EXPECT_NEAR(d3, 0.1 * sqrt(6), eps);
}