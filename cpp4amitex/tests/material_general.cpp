#include <gtest/gtest.h>

#include "amitex/errors.hpp"
#include "amitex/input/grid.hpp"
#include "amitex/input/intvar.hpp"
#include "amitex/input/materials.hpp"

#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

TEST(Materials, IntVars) {
  IntVar ivar0{0.1};
  EXPECT_STREQ(toXMLString(ivar0).c_str(), R"(<IntVar Index="0" Type="Constant" Value="0.1"/>)");

  IntVar ivar1{std::vector{0.2}};
  EXPECT_STREQ(toXMLString(ivar1).c_str(), R"(<IntVar Index="0" Type="Constant" Value="0.2"/>)");

  IntVar ivar2{std::vector{0.2, 0.3}};
  ivar2.setFile("ivar2.bin");
  EXPECT_STREQ(toXMLString(ivar2).c_str(),
               R"(<IntVar Index="0" Type="Constant_Zone" File="ivar2.bin" Format="Binary"/>)");

  IntVar ivar3{Field{{2, 2, 2}}};
  ivar3.setFile("ivar3.vtk");
  EXPECT_STREQ(toXMLString(ivar3).c_str(),
               R"(<IntVar Index="0" Type="Variable" File="ivar3.vtk" Format="vtk"/>)");

  Material mat;
  mat.addIntVar(ivar3.field);
  mat.addIntVar(300.0);
  mat.intVar(0).setFile("ivar1.vtk");
  EXPECT_STREQ(
      toXMLString(mat).c_str(),
      R"(<Material numM="0"><IntVar Index="1" Type="Variable" File="ivar1.vtk" Format="vtk"/><IntVar Index="2" Type="Constant" Value="300"/></Material>)");
}

TEST(Materials, Composite) {
  MaterialComposite composite;
  composite.setDirectory("my_composite_dir");
  EXPECT_STREQ(
      toXMLString(composite).c_str(),
      R"(<Material_composite><Coeff_composite directory="my_composite_dir"/></Material_composite>)");
}

std::tuple<Material, Zone> prepareMustFillZoneCoeff() {
  Grid grid{{2, 2, 1}, {1., 1., 1.}};

  Material mat;
  mat.setLaw("elasiso");
  Zone zone0(grid.dims(), {{0, 0, 0}, {0, 1, 0}});
  Zone zone1(grid.dims(), {{1, 0, 0}, {1, 1, 0}});
  mat.setNumberCoeff(2);
  mat.setNumberCoeffK(1);
  mat.setNumberCoeffComposite(2);
  mat.addZone(zone0, {1., 2.}, {1.}, {1., 2.});
  return {mat, zone1};
}

TEST(Materials, MustFillZoneCoeff) {
  auto [mat, zone1] = prepareMustFillZoneCoeff();
  // InputError: addZone: Once the coeff of the 1st zone is set, those of other zones must be set
  ASSERT_THROW(mat.addZone(zone1, {}, {}, {});, InputError);
}

TEST(Materials, MustFillZoneCoeffK) {
  auto [mat, zone1] = prepareMustFillZoneCoeff();
  // InputError: addZone: Once the coeff of the 1st zone is set, those of other zones must be set
  ASSERT_THROW(mat.addZone(zone1, {1., 2.}, {}, {});, InputError);
}

TEST(Materials, MustFillZoneCoeffComposite) {
  auto [mat, zone1] = prepareMustFillZoneCoeff();
  // InputError: addZone: Once the coeff of the 1st zone is set, those of other zones must be set
  ASSERT_THROW(mat.addZone(zone1, {1., 2.}, {1.}, {});, InputError);
}
