#include <algorithm>
#include <cassert>
#include <set>

#include <gtest/gtest.h>

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

Ptr<Zone> makeRectangle(GridPoint begin, GridPoint end, GridPoint dims) {
  auto zone = Zone::create(dims);
  for (size_t ix = begin[0]; ix < end[0]; ix++) {
    for (size_t iy = begin[1]; iy < end[1]; iy++) {
      for (size_t iz = begin[2]; iz < end[2]; iz++) {
        zone->add({ix, iy, iz});
      }
    }
  }
  return zone;
}
//! Define a simulation with 2 materials and 2 zone/mat
void addZoneAndLaunchBilayer() {
  constexpr size_t N = 40;
  constexpr double DX = 0.025;
  Grid grid{{N, N, N}, {DX, DX, DX}};

  double lambda[2][2] = {{100.e6, 200.e6}, {300.e6, 400.e6}};
  double mu[2][2] = {{100.e6, 200.e6}, {300.e6, 400.e6}};

  auto algo = Algorithm::create("Basic_Scheme", true);
  auto meca = Mechanics::create("Default", true);
  auto algoParams = AlgorithmParameters::create(algo, meca);

  auto loading = Loading::create();
  loading->setTimeDiscretizationLinear(1, 100.0);
  loading->setLinearEvolution(Component::XX, MechanicDriving::Strain, 0.01);
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      if (i != 0 || j != 0) loading->setLinearEvolution({i, j}, MechanicDriving::Stress, 0.0);
    }
  }
  auto loadingOutput = LoadingOutput::create();
  loadingOutput->add(loading);

  auto materials = Materials::create();
  materials->referenceMaterial = ReferenceMaterial::create(250e6, 250e6);

  for (size_t m = 0; m < 2; m++) {
    auto mat = Material::create();
    mat->setLaw("elasiso");
    mat->setNumberCoeff(2);

    auto minY = m == 0 ? N / 2 : 0;
    auto maxY = m == 0 ? N : N / 2;

    for (size_t z = 0; z < 2; z++) {
      auto minZ = z == 0 ? N / 2 : 0;
      auto maxZ = z == 0 ? N : N / 2;
      auto zone = makeRectangle({0, minY, minZ}, {N, maxY, maxZ}, grid.dims());
      mat->addZone(zone, {lambda[m][z], mu[m][z]});
    }

    materials->add(mat);
  }
  auto input =
      Input::create(grid, std::move(algoParams), std::move(materials), std::move(loadingOutput));
  input->resultsDir = "testresults/addzoneandlaunchbilayer";

  input->generateFiles();

  std::filesystem::path refDir = "ref-amxdir/bilayer";
  std::filesystem::path dir = input->resultsDir;

  EXPECT_TRUE(compareXMLFiles(dir / "materials.xml", refDir / "materials.xml"));
  EXPECT_TRUE(compareXMLFiles(dir / "algorithm.xml", refDir / "algorithm.xml"));
  EXPECT_TRUE(compareXMLFiles(dir / "loading.xml", refDir / "loading.xml"));

  EXPECT_TRUE(compareVtkWithRef(dir / "materialIds.vtk", refDir / "materialIds.vtk", 0.0));
  EXPECT_TRUE(compareVtkWithRef(dir / "zoneIds.vtk", refDir / "zoneIds.vtk", 0.0));

  for (int i = 1; i <= 2; i++) {
    for (int j = 1; j < 2; j++) {
      std::stringstream fname;
      fname << "Coeff" << i << '_' << j << ".bin";
      EXPECT_TRUE(compareBinWithRef(dir / fname.str(), refDir / fname.str(), 0.0));
    }
  }
}

TEST(Input, Bilayer) { addZoneAndLaunchBilayer(); }