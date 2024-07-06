#include <gtest/gtest.h>

#include "amitex/input/algorithm_parameters.hpp"
#include "amitex/input/loading.hpp"
#include "amitex/input/loading_output.hpp"

#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

constexpr char algorithmStr[] =
    R"(<Algorithm Type="Basic_Scheme"><Convergence_Criterion Value="0.2"/><Convergence_Acceleration Value="False"/><Nitermax Value="36"/><Convergence_Criterion_Compatibility Value="1e-09"/></Algorithm>)";

constexpr char diffusionStr[] =
    R"(<Diffusion><Filter Type="Default"/><Stationary Value="True"/></Diffusion>)";

constexpr char mechanicsStr[] =
    R"(<Mechanics><Filter Type="hexa"/><Small_Perturbations Value="True"/><C0Sym Value="False"/></Mechanics>)";

TEST(ParamAlgorithm, Algorithm) {
  Algorithm algo{"Basic_Scheme", false};
  EXPECT_STREQ(algo.type.c_str(), "Basic_Scheme");
  algo.convergenceAcceleration = true;
  algo.convergenceAcceleration.value = false;
  EXPECT_FALSE(algo.convergenceAcceleration);
  algo.convergenceCriterion.value = 0.3;
  EXPECT_EQ(algo.convergenceCriterion, 0.3);
  algo.convergenceCriterion = 0.2;
  EXPECT_EQ(algo.convergenceCriterion, 0.2);
  algo.nitermax = 36;
  EXPECT_EQ(algo.nitermax, 36);
  algo.convergenceCriterionCompatibility = 1.0e-9;
  EXPECT_EQ(algo.convergenceCriterionCompatibility, 1.0e-9);
  EXPECT_STREQ(toXMLString(algo).c_str(), algorithmStr);

  algo.convergenceAcceleration.modACV = 2;
  EXPECT_EQ(algo.convergenceAcceleration.modACV, 2);
  EXPECT_STREQ(toXMLString(algo.convergenceAcceleration).c_str(),
               R"(<Convergence_Acceleration Value="False" modACV="2"/>)");

  algo.convergenceCriterionSmacro = 1.0e-3;
  EXPECT_EQ(algo.convergenceCriterionSmacro, 1.0e-3);
  EXPECT_STREQ(toXMLString(algo.convergenceCriterionSmacro).c_str(),
               R"(<Convergence_Criterion_Smacro Value="0.001"/>)");
  algo.nitermin = 0;
  EXPECT_STREQ(toXMLString(algo.nitermin).c_str(), R"(<Nitermin Value="0"/>)");
  algo.niterminACV = 0;
  EXPECT_STREQ(toXMLString(algo.niterminACV).c_str(), R"(<Nitermin_acv Value="0"/>)");
  algo.initialize = "previous";
  EXPECT_STREQ(algo.initialize.value.value().c_str(), "previous");
  EXPECT_STREQ(toXMLString(algo.initialize).c_str(), R"(<Initialize Value="previous"/>)");

  Substepping sub;
  sub.nitermax2 = 100;
  sub.geomRatio = 1.0;
  sub.nsub = 10;
  sub.depth = 3;
  EXPECT_STREQ(toXMLString(sub).c_str(),
               R"(<Substepping Nitermax2="100" Geom_ratio="1" Nsub="10" Depth="3"/>)");
}

TEST(ParamAlgorithm, Diffusion) {
  Diffusion diffu{"Defau", false};
  diffu.filter.type = "Default";
  EXPECT_STREQ(diffu.filter.type.c_str(), "Default");
  diffu.stationary = true;
  EXPECT_TRUE(diffu.stationary);
  EXPECT_STREQ(toXMLString(diffu).c_str(), diffusionStr);
}

TEST(ParamAlgorithm, Mechanics) {
  Mechanics meca;
  meca.filter.type = "hexa";
  EXPECT_STREQ(meca.filter.type.c_str(), "hexa");
  meca.smallPerturbations = true;
  EXPECT_TRUE(meca.smallPerturbations);
  meca.C0Sym = false;
  EXPECT_FALSE(meca.C0Sym);
  EXPECT_STREQ(toXMLString(meca).c_str(), mechanicsStr);

  meca.smallPerturbations.displacementGradient = "nsym";
  EXPECT_STREQ(toXMLString(meca.smallPerturbations).c_str(),
               R"(<Small_Perturbations Value="True" Displacement_Gradient="nsym"/>)");
}

TEST(Loading, DiffusionLinear) {
  Loading load;
  load.setTimeDiscretizationUser({1, 2, 2.6});
  for (int i = 0; i < 3; i++) {
    load.setEvolution(i, DiffusionDriving::Gradient, Evolution::Linear, 0.0);
  }
  EXPECT_STREQ(
      toXMLString(load).c_str(),
      R"(<Loading Tag="0"><Time_Discretization Discretization="User" Nincr="3"/><Time_List>1 2 2.6 </Time_List><x0 Driving="GradD" Evolution="Linear" Value="0"/><y0 Driving="GradD" Evolution="Linear" Value="0"/><z0 Driving="GradD" Evolution="Linear" Value="0"/></Loading>)");
}

TEST(Loading, TimeLinear) {
  Loading load;
  load.setTimeDiscretizationLinear(11, 10.0);
  EXPECT_STREQ(
      toXMLString(load).c_str(),
      R"(<Loading Tag="0"><Time_Discretization Discretization="Linear" Nincr="11" Tfinal="10"/></Loading>)");
}

TEST(Loading, MechanicConstant) {
  Loading load;
  load.setTimeDiscretizationLinear(1, 1.0);
  load.setConstantEvolution(Component::XX, MechanicDriving::Strain, 0.3);
  load.setDirStress(DirStress::Cauchy);
  EXPECT_STREQ(
      toXMLString(load).c_str(),
      R"(<Loading Tag="0"><Time_Discretization Discretization="Linear" Nincr="1" Tfinal="1"/><xx Driving="Strain" Evolution="Constant" DirStress="0.3"/><DirStress Type="Cauchy"/></Loading>)");
}

TEST(Loading, LoadExt) {
  Loading load;
  load.setTimeDiscretizationLinear(1, 1.0);
  load.setTemperatureEvolution(Evolution::Constant);
  load.setParamEvolution(0, Evolution::Linear, 50);
  load.setParamEvolution(1, Evolution::Linear, 0);
  EXPECT_STREQ(
      toXMLString(load).c_str(),
      R"(<Loading Tag="0"><Time_Discretization Discretization="Linear" Nincr="1" Tfinal="1"/><T Evolution="Constant"/><Param Index="1" Evolution="Linear" Value="50"/><Param Index="2" Evolution="Linear" Value="0"/></Loading>)");
}

TEST(Loading, InitLoadExt) {
  InitLoadExt init;
  init.temperature = 20.0;
  init.setParam(0, 0);
  init.setParam(1, 10);
  constexpr char initRef[] =
      R"(<InitLoadExt><T Value="20"/><Param Index="1" Value="0"/><Param Index="2" Value="10"/></InitLoadExt>)";

  EXPECT_STREQ(toXMLString(init).c_str(), initRef);

  LoadingOutput loadout;
  loadout.initLoadExt = init;
  EXPECT_STREQ(
      toXMLString(loadout).c_str(),
      (std::string{"<Loading_Output><Output></Output>"} + initRef + "</Loading_Output>").c_str());
}
