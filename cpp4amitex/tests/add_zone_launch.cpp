#include <algorithm>
#include <cassert>
#include <filesystem>
#include <set>

#include <gtest/gtest.h>

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
  input.generateFiles();

  std::filesystem::path refDir =
      std::string{"ref-amxdir/addzoneandlaunchthermal_"} + std::to_string(nbMat);
  std::filesystem::path dir = input.resultsDir;

  EXPECT_TRUE(compareXMLFiles(dir / "materials.xml", refDir / "materials.xml"));
  EXPECT_TRUE(compareXMLFiles(dir / "algorithm.xml", refDir / "algorithm.xml"));
  EXPECT_TRUE(compareXMLFiles(dir / "loading.xml", refDir / "loading.xml"));

  EXPECT_TRUE(compareVtkWithRef(dir / "materialIds.vtk", refDir / "materialIds.vtk", 0));
  EXPECT_TRUE(compareVtkWithRef(dir / "zoneIds.vtk", refDir / "zoneIds.vtk", 0));

  EXPECT_TRUE(compareBinWithRef(dir / "CoeffK1_1.bin", refDir / "CoeffK1_1.bin", 1.e-8));
}

TEST(Input, AddZoneThermal) {
  addZoneAndLaunchThermal(1);
  addZoneAndLaunchThermal(2);
}