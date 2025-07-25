#include <gtest/gtest.h>

#include "amitex/input.hpp"

#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

TEST(XML, AlgoDefault) {
  auto algo = Algorithm::create("Basic_Scheme", true);

  auto meca = Mechanics::create("Default", true);
  ;
  meca->filter = "Default";
  meca->smallPerturbations = true;

  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->algorithm = algo;
  param_algo->mechanics = meca;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_default.xml"));
}

TEST(XML, AlgoNoAC) {
  auto algo = Algorithm::create("Basic_Scheme", false);

  auto meca = Mechanics::create("Default", true);
  ;
  meca->filter = "Default";
  meca->smallPerturbations = true;

  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->algorithm = algo;
  param_algo->mechanics = meca;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_no_AC.xml"));
}

TEST(XML, AlgoDefaultCompatibility) {
  auto algo = Algorithm::create("Basic_Scheme", true);
  algo->nitermax = 2000;
  algo->convergenceCriterionCompatibility = 1.0e-10;

  auto meca = Mechanics::create("Default", true);
  ;
  meca->filter = "Default";
  meca->smallPerturbations = true;

  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->algorithm = algo;
  param_algo->mechanics = meca;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_default_compatibility.xml"));
}

TEST(XML, AlgoDefaultInitprevious) {
  auto algo = Algorithm::createDefault();
  algo->type = "Basic_Scheme";
  algo->convergenceAcceleration = true;
  algo->initialize = "previous";

  auto meca = Mechanics::create("Default", true);
  ;
  meca->filter = "Default";
  meca->smallPerturbations = true;

  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->algorithm = algo;
  param_algo->mechanics = meca;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_default_initprevious.xml"));
}

TEST(XML, AlgoDefaultModACV) {
  auto algo = Algorithm::createDefault();
  algo->type = "Basic_Scheme";
  algo->convergenceAcceleration = true;
  algo->convergenceAcceleration.modACV = 2;

  auto meca = Mechanics::create("Default", true);
  ;
  meca->filter = "Default";
  meca->smallPerturbations = true;

  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->algorithm = algo;
  param_algo->mechanics = meca;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_default_modACV.xml"));
}

TEST(XML, AlgoDefaultNitermin) {
  auto algo = Algorithm::createDefault();
  algo->type = "Basic_Scheme";
  algo->convergenceAcceleration = true;
  algo->nitermin = 0;
  algo->niterminACV = 0;

  auto meca = Mechanics::create("Default", true);
  ;
  meca->filter = "Default";
  meca->smallPerturbations = true;

  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->algorithm = algo;
  param_algo->mechanics = meca;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_default_Nitermin.xml"));
}

TEST(XML, AlgoDefaultSubstep) {
  auto algo = Algorithm::create("Basic_Scheme", true);
  algo->type = "Basic_Scheme";
  algo->convergenceAcceleration = true;
  algo->convergenceCriterion = 1.e-4;
  algo->nitermax = 1000;

  auto substep = Substepping::create();
  substep->nitermax2 = 100;
  substep->geomRatio = 1.;
  substep->nsub = 10;
  substep->depth = 3;
  algo->substepping = substep;

  auto meca = Mechanics::create("Default", true);
  ;
  meca->filter = "Default";
  meca->smallPerturbations = false;

  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->algorithm = algo;
  param_algo->mechanics = meca;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_default_substep.xml"));
}

TEST(XML, AlgoOld) {
  auto algo = Algorithm::create("Basic_Scheme", true);
  algo->type = "Basic_Scheme";
  algo->convergenceCriterion = 1.e-5;
  algo->convergenceAcceleration = false;

  auto meca = Mechanics::create("Default", true);
  ;
  meca->filter = "no_filter";
  meca->smallPerturbations = true;

  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->algorithm = algo;
  param_algo->mechanics = meca;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_old.xml"));
}

TEST(XML, AlgoOcta) {
  auto algo = Algorithm::create("Basic_Scheme", true);
  algo->type = "Basic_Scheme";
  algo->convergenceAcceleration = true;

  auto meca = Mechanics::create("Default", true);
  ;
  meca->filter = "Octa";
  meca->smallPerturbations = true;

  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->algorithm = algo;
  param_algo->mechanics = meca;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_octa.xml"));
}

TEST(XML, AlgoSmacroGD) {
  auto algo = Algorithm::create("Basic_Scheme", true);
  algo->type = "Basic_Scheme";
  algo->convergenceCriterionSmacro = 1.e-3;
  algo->convergenceAcceleration = true;

  auto meca = Mechanics::create("Default", true);

  meca->filter = "Default";
  meca->smallPerturbations = false;

  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->algorithm = algo;
  param_algo->mechanics = meca;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_Smacro_GD.xml"));
}

TEST(XML, AlgoDefaultCVFor) {
  auto algo = Algorithm::create("Basic_Scheme", true);
  algo->type = "Basic_Scheme";
  algo->convergenceAcceleration = true;
  ConvergenceForced cvfor = true;
  cvfor.nitCVFor = 100;
  cvfor.nCVFor = 101;
  algo->convergenceForced = cvfor;

  auto meca = Mechanics::create("Default", true);
  meca->filter = "Default";
  meca->smallPerturbations = true;

  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->algorithm = algo;
  param_algo->mechanics = meca;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_default_cvfor.xml"));
}

TEST(XML, AlgoDefaultVoxcomplaminate) {
  auto algo = Algorithm::createDefault();
  algo->type = "Basic_Scheme";
  algo->convergenceAcceleration = true;

  auto meca = Mechanics::create("Default", true);

  auto laminate = AlgorithmLaminate::create();
  laminate->convergenceAcceleration = true;
  laminate->initializationType = "Linear";
  laminate->nIncrements = 1;

  auto param_algo = AlgorithmParameters::create(algo, meca);
  param_algo->algorithmLaminate = laminate;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algo_default_voxcomplaminate.xml"));
}

TEST(XML, CharTraction1) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->setVtkStressStrain(1, 1);
  lo->output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(1, 100.);
  loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.01);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      if (i != 0 || j != 0) loading->setLinearEvolution({i, j}, MechanicDriving::Stress, 0.);
    }
  }

  lo->add(loading);

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_traction_1.xml"));
}

TEST(XML, CharTractionGD) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 1);
  lo->output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(500, 500.);
  loading->setLinearEvolution(Component::XX, MechanicDriving::Stress, 0.0);
  loading->setLinearEvolution(Component::YY, MechanicDriving::Stress, 0.0);
  loading->setLinearEvolution(Component::ZZ, MechanicDriving::Strain, 0.05);
  loading->setLinearEvolution(Component::XY, MechanicDriving::Stress, 0.0);
  loading->setLinearEvolution(Component::XZ, MechanicDriving::Stress, 0.0);
  loading->setLinearEvolution(Component::YZ, MechanicDriving::Stress, 0.0);
  loading->setLinearEvolution(Component::YX, MechanicDriving::Strain, 0.0);
  loading->setLinearEvolution(Component::ZX, MechanicDriving::Strain, 0.0);
  loading->setLinearEvolution(Component::ZY, MechanicDriving::Strain, 0.0);
  lo->add(loading);

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_traction_GD.xml"));
}

TEST(XML, CharTractionPolyXGD) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 1);
  lo->output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(2000, 2000.);
  loading->setOutputVtkList({2000});
  loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.2);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      if (i != 0 || j != 0) loading->setConstantEvolution({i, j}, MechanicDriving::Stress);
    }
  }
  for (int j = 0; j < 3; j++) {
    for (int i = j + 1; i < 3; i++) {
      loading->setConstantEvolution({i, j}, MechanicDriving::Strain);
    }
  }
  lo->add(loading);

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_traction_polyx_GD.xml"));
}

TEST(XML, CharThermoBicouche) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 1);
  lo->output = output;

  auto initload = InitLoadExt::create();
  initload->temperature = 0.01;
  lo->initLoadExt = initload;

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(10, 100.);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      loading->setLinearEvolution({i, j}, MechanicDriving::Strain, 0.);
    }
  }
  loading->setTemperatureEvolution(Evolution::Constant);
  lo->add(loading);

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_thermo_bicouche.xml"));
}

TEST(XML, CharParamExtBicouche) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 1);
  lo->output = output;

  auto initload = InitLoadExt::create();
  initload->setParam(0, 0.01);
  lo->initLoadExt = initload;

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(10, 100.);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      loading->setLinearEvolution({i, j}, MechanicDriving::Strain, 0.);
    }
  }
  loading->setParamEvolution(0, Evolution::Constant);
  lo->add(loading);

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_paramext_bicouche.xml"));
}

TEST(XML, CharDefImp) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 1);
  lo->output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationUser({32832});
  loading->setOutputVtkList({1});
  loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, -5e-4);
  loading->setLinearEvolution(Component::YY, MechanicDriving::Strain, 1.5e-4);
  loading->setLinearEvolution(Component::ZZ, MechanicDriving::Strain, 1.5e-4);
  loading->setLinearEvolution(Component::XY, MechanicDriving::Strain, 0.0);
  loading->setLinearEvolution(Component::XZ, MechanicDriving::Strain, 0.0);
  loading->setLinearEvolution(Component::YZ, MechanicDriving::Strain, 0.0);
  lo->add(loading);

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_def_imp.xml"));
}

TEST(XML, CharCycleTraction) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 1);
  output->addZone(0, {0});
  lo->output = output;

  for (int i = 0; i < 3; i++) {
    auto loading = Loading::create();
    loading->setTimeDiscretizationLinear(i == 0 ? 50 : 100, 100.0 * i + 50.0);
    loading->setOutputVtkList({1});
    if (i < 2) loading->setOutputCell(10 * (i + 1));
    loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, i % 2 == 0 ? 5e-3 : -5e-3);
    for (int i = 0; i < 3; i++) {
      for (int j = i; j < 3; j++) {
        if (i != 0 || j != 0) loading->setLinearEvolution({i, j}, MechanicDriving::Stress, 0.0);
      }
    }
    lo->add(loading);
  }

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_cycle_traction.xml"));
}

static void prepareTriax2p5(LoadingOutput& lo, DirStress ds) {
  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(0, 0);
  lo.output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(40, 40.);
  loading->setOutputVtkList({1});
  loading->setConstantEvolution(Component::XX, MechanicDriving::Stress, 1.);
  loading->setConstantEvolution(Component::YY, MechanicDriving::Stress, 1.3);
  loading->setLinearEvolution(Component::ZZ, MechanicDriving::Strain, 4e-3, 1.6);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (i != j) loading->setConstantEvolution({i, j}, MechanicDriving::Stress, 0.0);
    }
  }

  loading->setDirStress(ds);
  lo.add(loading);
}

TEST(XML, CharTriax2p5PK1) {
  auto lo = LoadingOutput::create();
  prepareTriax2p5(*lo, DirStress::PK1);
  EXPECT_TRUE(compareXMLWithRef(*lo, "char_triax2p5_PK1.xml"));
}

TEST(XML, CharTriax2p5Cauchy) {
  auto lo = LoadingOutput::create();
  prepareTriax2p5(*lo, DirStress::Cauchy);
  EXPECT_TRUE(compareXMLWithRef(*lo, "char_triax2p5_cauchy.xml"));
}

TEST(XML, CharDiffusionFlux) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->setVtkFluxDGradD(1, 1);
  output->addZone(Output::Zone{0});
  lo->output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationUser({1});
  loading->setOutputVtkList({1});
  loading->setOutputZone(1);

  loading->setLinearEvolution(Component::X, DiffusionDriving::Flux, 1.);
  loading->setLinearEvolution(Component::Y, DiffusionDriving::Flux, 2.);
  loading->setLinearEvolution(Component::Z, DiffusionDriving::Flux, 3.);
  lo->add(loading);
  EXPECT_TRUE(compareXMLWithRef(*lo, "char_diffusion_flux.xml"));
}

const std::array<double, 31> someTimes = {
    83808.0,  143424,   211680,   289440,   380160,   483840,   604800,  738720,
    898560,   1080000,  1287360,  1529280,  1801440,  2116800,  2479680, 2903040,
    3386880,  3939840,  4579200,  5313600,  6160320,  7128000,  8251200, 9538560,
    11016000, 12718080, 14670720, 16934400, 19526400, 22464000, 25920000};

TEST(XML, CharFluage) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 1);
  output->addZone(Output::Zone{1});
  lo->output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationUser({32832});
  loading->setOutputZone(1);

  loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.01);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      if (i != 0 || j != 0) {
        loading->setLinearEvolution({i, j}, MechanicDriving::Stress, 0.);
      }
    }
  }
  lo->add(loading);

  auto loading2 = Loading::create();
  loading2->setTimeDiscretizationUser({someTimes.begin(), someTimes.end()});

  loading2->setOutputVtkList({31});
  loading2->setOutputZone(30);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      loading2->setConstantEvolution({i, j}, MechanicDriving::Stress);
    }
  }
  lo->add(loading2);

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_fluage.xml"));
}

TEST(XML, CharOutputFullOptions) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 0);
  Output::Zone zone;
  zone.numM = 0;
  zone.setVarIntList({0});
  output->addZone(zone);
  output->addZone({2});
  output->addVtkIntVarList(0, {2});
  output->addVtkIntVarList(1, {0, 1});
  lo->output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(1000, 10.);
  loading->setOutputCell(100);
  loading->setOutputZone(10);
  loading->setOutputVtkList({500, 1000});
  loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.01);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      if (i != 0 || j != 0) loading->setLinearEvolution({i, j}, MechanicDriving::Stress, 0.0);
    }
  }
  lo->add(loading);
  EXPECT_TRUE(compareXMLWithRef(*lo, "char_output_full_options.xml"));
}

TEST(XML, CharFluage2) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 1);
  output->addZone({1});
  lo->output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationUser({32832});
  loading->setOutputZone(1);
  loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.012);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      if (i != 0 || j != 0) loading->setLinearEvolution({i, j}, MechanicDriving::Stress, 0.0);
    }
  }
  loading->addUserInterruptValue(0.42706742E+09);
  lo->add(loading);

  auto loading2 = Loading::create();
  loading2->setTimeDiscretizationUser({someTimes.begin(), someTimes.end()});
  loading2->setOutputVtkList({31});
  loading2->setOutputZone(30);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      loading2->setConstantEvolution({i, j}, MechanicDriving::Stress);
    }
  }
  lo->add(loading2);

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_fluage2.xml"));
}

TEST(XML, CharDefimpFlexionBeam) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 1);
  lo->output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(1, 100.);
  loading->setOutputVtkList({1});
  loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.01);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      if (i != 0 || j != 0) loading->setLinearEvolution({i, j}, MechanicDriving::Strain, 0.0);
    }
  }
  loading->setGradGradU(Component::XX, Component::Y, Evolution::Linear, 0.001);
  loading->setGradGradU(Component::YY, Component::Y, Evolution::Linear, -0.0003);
  loading->setGradGradU(Component::ZZ, Component::Y, Evolution::Linear, -0.0003);
  lo->add(loading);

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_defimp_flexion_beam.xml"));
}

TEST(XML, CharDefimpTorsionBeam) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 1);
  lo->output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(1, 100.);
  loading->setOutputVtkList({1});
  loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.01);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      if (i != 0 || j != 0) loading->setLinearEvolution({i, j}, MechanicDriving::Strain, 0.0);
    }
  }
  loading->setGradGradU(Component::ZX, Component::Y, Evolution::Linear, 0.002);
  loading->setGradGradU(Component::YX, Component::Z, Evolution::Linear, -0.002);
  lo->add(loading);

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_defimp_torsion_beam.xml"));
}

TEST(XML, MatBeton) {
  auto materials = Materials::create();

  auto ref = ReferenceMaterial::create(2.0952e+10, 1.5014e+10);
  materials->referenceMaterial = ref;

  auto mat = Material::create();
  mat->setLaw("viscoelas_maxwell");
  mat->setCoeffs({70e9, 0.3, 4, 3.58873e9, 3.10474e9, 6.4781e9, 3.0942e9, 172800, 7.51552e9,
                  3.15347e9, 1728000, 5.10283e9, 3.15347e9, 17280000});
  mat->setCoeffName(10, "NameCoeffbidon11");

  for (int i = 0; i < 36; i++) {
    mat->addIntVar(0.);
  }
  mat->setIntVarName(2, "NameVIbidon3");
  materials->add(mat);

  auto mat2 = Material::create();
  mat2->setLaw("elasiso");
  mat2->setCoeffs({4.0385e+10, 2.6923e+10});
  materials->add(mat2);

  EXPECT_TRUE(compareXMLWithRef(*materials, "mat_beton.xml"));
}

TEST(XML, ThermMerop0) {
  auto algo = Algorithm::create("Basic_Scheme", true);
  algo->nitermax = 3000;
  auto diffu = Diffusion::create("Default", false);
  diffu->filter = "Default";
  diffu->stationary = true;
  auto param_algo = AlgorithmParameters::create(algo);
  param_algo->diffusion = diffu;
  EXPECT_TRUE(compareXMLWithRef(*param_algo, "algorithm-0.xml"));

  auto materials = Materials::create();
  materials->referenceMaterialD = ReferenceMaterialD::create(214.8);
  const double kappas[] = {0.6, 429.0};
  for (int i = 0; i < 2; i++) {
    auto mat = Material::create();
    mat->setLawK("Fourier_iso_polarization");
    mat->setCoeffKs({kappas[i], 0.0, 0.0, -kappas[i]});
    materials->add(std::move(mat));
  }
  EXPECT_TRUE(compareXMLWithRef(*materials, "material-0.xml"));

  auto loading = LoadingOutput::create();
  loading->output->vtkFluxDGradD = VtkFluxDGradD::create(0, 0);
  auto load = Loading::create();
  double times[] = {1, 2};
  load->setTimeDiscretizationUser(2, times);
  for (int i = 0; i < 3; i++) {
    load->setEvolution(i, DiffusionDriving::Gradient, Evolution::Linear, 0.0);
  }
  loading->add(std::move(load));
  EXPECT_TRUE(compareXMLWithRef(*loading, "loading-0.xml"));
}

TEST(XML, CharFluageRestart) {
  auto lo = LoadingOutput::create();

  auto output = Output::create();
  output->vtkStressStrain = VtkStressStrain::create(1, 1);
  output->addZone(Output::Zone{1});
  lo->output = output;

  auto loading = Loading::create();
  loading->setTimeDiscretizationUser({32832});
  loading->setOutputZone(1);

  loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.01);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      if (i != 0 || j != 0) {
        loading->setLinearEvolution({i, j}, MechanicDriving::Stress, 0.);
      }
    }
  }
  lo->add(loading);

  auto loading2 = Loading::create();
  loading2->setTimeDiscretizationUser({someTimes.begin(), someTimes.end()});
  loading2->setRestart(10);
  loading2->setOutputVtkList({31});
  loading2->setOutputZone(30);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      loading2->setConstantEvolution({i, j}, MechanicDriving::Stress);
    }
  }
  lo->add(loading2);

  EXPECT_TRUE(compareXMLWithRef(*lo, "char_fluage_restart.xml"));
}