#include <gtest/gtest.h>

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

#include "testutils.hpp"

// Test with a law containing some internal variables

using namespace amitex;
using namespace amitex_tests;

TEST(IntVar, ElasIsoEigs) {
  Grid grid{{32, 32, 32}, {1, 1, 1}};

  auto algorithm = Algorithm::create("Basic_Scheme", false);
  auto mechanics = Mechanics::create("Default", true);
  auto algoParams = AlgorithmParameters::create(algorithm);
  algoParams->algorithm = algorithm;
  algoParams->mechanics = mechanics;

  auto loadingOutput = LoadingOutput::create();
  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(10, 1.e-11);
  loading->setLinearEvolution(Component::XX, MechanicDriving::Stress, 20.0e6);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (i != 0 || j != 0) loading->setLinearEvolution({i, j}, MechanicDriving::Stress, 0.0);
    }
  }
  loadingOutput->add(loading);

  auto materials = Materials::create();
  constexpr double lambda = 26.235e+10, mu = 42.00e+09;
  materials->referenceMaterial = ReferenceMaterial::create(lambda, mu);
  auto material = Material::create();
  material->setLaw("elasiso_eigs");
  material->setCoeffs({lambda, mu});
  Field<double> freeStr0{grid.dims()};
  freeStr0.fill(0.);
  material->addIntVar(freeStr0);
  for (int i = 1; i < 6; i++) material->addIntVar(0);
  material->addZone(Zone::create(grid.dims(), grid.allPoints().begin(), grid.allPoints().end()));
  materials->add(material);

  auto input = Input::create(grid, algoParams, materials, loadingOutput);
  input->resultsDir = "testresults/elaiso_eigs";

  input->generateFiles();

  std::filesystem::path refDir = "ref-amxdir/elaiso_eigs";
  std::filesystem::path dir = input->resultsDir;

  EXPECT_TRUE(compareXMLFiles(dir / "materials.xml", refDir / "materials.xml"));
  EXPECT_TRUE(compareXMLFiles(dir / "algorithm.xml", refDir / "algorithm.xml"));
  EXPECT_TRUE(compareXMLFiles(dir / "loading.xml", refDir / "loading.xml"));

  EXPECT_TRUE(compareVtkWithRef(dir / "intvar_1_1.vtk", refDir / "intvar_1_1.vtk", 1.e-8));
}
