#include <algorithm>
#include <cassert>
#include <set>

#include <gtest/gtest.h>
#include <mpi.h>

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

#include "mat2zone2_prepare.hpp"
#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

//! Define a simulation with 2 materials and 1 mat/zone or
//! \param nbMat 1 or 2
static void addZoneAndLaunchThermal(size_t nbMat) {
  Input input = mat2zone2Prepare(nbMat);

  ASSERT_NO_THROW(runSimulation(input));

  Extract ext{input.outputPrefix()};
  auto flux = ext.averageDiffusionFlux(0);
  auto grad = ext.averageDiffusionGradient(0);
  constexpr double fluxRef[] = {-0.30825423E-04, 0.30825423E-04, -0.84332291E+00};

  for (size_t i = 0; i < 3; i++) {
    EXPECT_NEAR(flux[i], fluxRef[i], eps);
  }
  for (size_t i = 0; i < 3; i++) {
    EXPECT_NEAR(grad[i], 0.0, eps);
  }
}

TEST(Input, Mat2Zone2Run) {
  MPI_Init(nullptr, nullptr);
  addZoneAndLaunchThermal(1);
  addZoneAndLaunchThermal(2);
  MPI_Finalize();
}