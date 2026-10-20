#include <cmath>
#include <utility>

#include <gtest/gtest.h>

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

// Sketch of material (along XY plane)
// |    /|
// | 0 / |
// |  /  |
// | / 1 |
// |/    |

constexpr std::array pureCoeffs0 = {100e6, 200e6};
constexpr std::array pureCoeffs1 = {150e6, 250e6};

constexpr GridSize dims = {5, 5, 5};
constexpr Vector3D voxL = {1.0, 1.0, 1.0};

Ptr<Input> makeInputCommon(Zone& zoneInter) {
  Grid grid{dims, voxL};
  auto algo = Algorithm::create("Basic_Scheme", true);
  auto meca = Mechanics::create("Default", true);
  auto paramAlgo = AlgorithmParameters::create(algo, meca);

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(4, 1.0);
  // Something along the interface
  loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.01);
  loading->setLinearEvolution(Component::XY, MechanicDriving::Strain, 0.01);
  loading->setLinearEvolution(Component::YY, MechanicDriving::Strain, 0.01);
  loading->setLinearEvolution(Component::XZ, MechanicDriving::Stress, 0.0);
  loading->setLinearEvolution(Component::YZ, MechanicDriving::Stress, 0.0);
  loading->setLinearEvolution(Component::ZZ, MechanicDriving::Stress, 0.0);
  auto loadingOutput = LoadingOutput::create();
  loadingOutput->add(loading);

  auto zone0 = Zone::create(dims), zone1 = Zone::create(dims);
  for (size_t ix = 0; ix < dims[0]; ix++) {
    for (size_t iy = 0; iy < dims[1]; iy++) {
      for (size_t iz = 0; iz < dims[2]; iz++) {
        GridPoint p = {ix, iy, iz};
        if (iy > ix) {
          zone0->add(p);
        } else if (ix == iy) {
          zoneInter.add(p);
        } else {
          zone1->add(p);
        }
      }
    }
  }
  // pure zones
  auto mat0 = Material::create();
  mat0->setLaw("elasiso");
  mat0->setNumberCoeff(2);
  mat0->setNumberCoeffComposite(2);
  mat0->addZone(zone0, {pureCoeffs0[0], pureCoeffs1[0]}, {}, {pureCoeffs0[0], pureCoeffs1[0]});
  auto mat1 = Material::create();
  mat1->setLaw("elasiso");
  mat1->setNumberCoeff(2);
  mat1->setNumberCoeffComposite(2);
  mat1->addZone(zone1, {pureCoeffs0[1], pureCoeffs1[1]}, {}, {pureCoeffs0[1], pureCoeffs1[1]});
  auto materials = Materials::create();
  materials->add(std::move(mat0));
  materials->add(std::move(mat1));

  materials->referenceMaterial = ReferenceMaterial::create(0.5 * (pureCoeffs0[0] + pureCoeffs0[1]),
                                                           0.5 * (pureCoeffs1[0] + pureCoeffs1[1]));

  return Input::create(grid, paramAlgo, materials, std::move(loadingOutput));
}

static void composite_prism() {
  const std::string& law = "reuss";
  auto zoneInter = Zone::create(dims);
  auto input = makeInputCommon(*zoneInter);

  input->materials->material(0)->addZone(zoneInter, {pureCoeffs0[0], pureCoeffs1[0]}, {},
                                         {pureCoeffs0[0], pureCoeffs1[0]});

  InterfaceGeometry geom{.normal = {-1. / sqrt(2.), 1. / sqrt(2.), 0.0},
                         .tangent = {1. / sqrt(2), 1. / sqrt(2), 0.},
                         .surface = sqrt(2)};
  auto inter = Composite::create({0, 1}, law);
  for (auto pos : zoneInter->linearPositions()) inter->addVoxel(pos, {0.5, 0.5}, {geom});

  EXPECT_EQ(inter->numberPhases(), 2);
  EXPECT_EQ(inter->positions().size(), 5 * 5);
  for (size_t i = 0; i < 2; i++) {
    for (auto x : inter->volumeFractions(i)) {
      EXPECT_EQ(x, 0.5);
    }
  }

  input->materials->composites.add(std::move(inter));

  input->resultsDir = "testresults/testcomposite_reuss";

  input->generateFiles();

  std::filesystem::path refDir = "ref-amxdir/testcomposite_reuss";
  std::filesystem::path dir = input->resultsDir;
  EXPECT_TRUE(compareXMLFiles(dir / "materials.xml", refDir / "materials.xml"));
  EXPECT_TRUE(compareXMLFiles(dir / "algorithm.xml", refDir / "algorithm.xml"));
  EXPECT_TRUE(compareXMLFiles(dir / "loading.xml", refDir / "loading.xml"));

  for (size_t i = 0; i < input->materials->composite(0)->tangents().at(0).size(); i++) {
    for (size_t j = 0; j < input->materials->composite(0)->tangents().at(0).at(i).size(); j++) {
      Vector3D T, N;
      for (size_t k = 0; k < 3; k++)
        T[k] = input->materials->composite(0)->tangents().at(k).at(i).at(j);
      for (size_t k = 0; k < 3; k++)
        N[k] = input->materials->composite(0)->normals().at(k).at(i).at(j);
      EXPECT_NEAR(N[0], -1. / sqrt(2), 1.e-8);
      EXPECT_NEAR(N[1], 1. / sqrt(2), 1.e-8);
      EXPECT_NEAR(N[2], 0.0, 1.e-8);
      double prod2 = 0.0;
      for (size_t k = 0; k < 3; k++) prod2 += T[k] * N[k];
      EXPECT_NEAR(prod2, 0.0, 1.e-8);
      double S = input->materials->composite(0)->surfaces().at(i).at(j);
      EXPECT_NEAR(S, sqrt(2), 1e-8);
    }
  }
}

TEST(Composite, SimplePrism) { composite_prism(); }
