#include <array>
#include <fstream>
#include <iostream>

#include <gtest/gtest.h>

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/input/material_builder.hpp"
#include "amitex/simulation.hpp"

#include "testutils.hpp"

using namespace amitex;
using namespace amitex_tests;

void compositeSpheres(bool useBuildAPI) {
  std::array<size_t, 3> N = {32, 32, 32};
  std::array<double, 3> DL = {10.0 / 32, 10.0 / 32, 10.0 / 32};
  Grid grid{N, DL};
  std::string law = "laminate";

  auto materials = Materials::create();

  {
    std::vector<std::tuple<VoxelSpec, Vector3D>> voxels;
    MaterialBuilder matBuilder;
    std::ifstream f{"data/simple_spheres_composite.txt"};
    for (auto p : grid.allPoints()) {
      VoxelSpec spec;
      size_t nphases;
      f >> nphases;
      for (size_t i = 0; i < nphases; i++) {
        size_t iphase;
        double phi;
        f >> iphase >> phi;
        spec.phases.push_back({iphase, phi, 0});
      }
      Vector3D normal;
      f >> normal[0] >> normal[1] >> normal[2];
      if (useBuildAPI) {
        voxels.push_back({spec, normal});
      } else {
        matBuilder.addVoxel(*materials, grid.linearize(p), spec, normal);
      }
    }
    if (useBuildAPI) {
      EXPECT_EQ(voxels.size(), grid.totalSize());
      buildMaterials(*materials, N, voxels, IndexOrdering::Fortran);
    }
  }

  std::vector<std::vector<double>> coeffs = {{1.0, 2.0}, {2.0, 3.0}};

  for (size_t i = 0; i < materials->numberMaterials(); i++) {
    std::cout << materials->material(i)->numberZones() << "\n";
    for (const auto& zone : materials->material(i)->zones()) {
      std::cout << "Mat " << i << " #zones = " << zone->numberVoxels() << "\n";
    }
    materials->material(i)->setLaw("elasiso");
    materials->material(i)->setCoeffs(coeffs[i]);
    materials->material(i)->setCoeffComposites(coeffs[i]);
  }

  for (size_t i = 0; i < materials->numberComposites(); i++) {
    materials->composite(i)->setLaw(law);
    std::cout << "Composite " << i << " #vox=" << materials->composite(i)->positions().size()
              << '\n';
  }

  materials->referenceMaterial = ReferenceMaterial::create(0.5 * (coeffs[0][0] + coeffs[1][0]),
                                                           0.5 * (coeffs[0][1] + coeffs[1][1]));

  auto algo = Algorithm::create("Basic_Scheme", true);
  algo->convergenceAcceleration = true;
  auto meca = Mechanics::createDefault();
  meca->filter = "Default";
  meca->smallPerturbations = true;
  auto algoPara = AlgorithmParameters::create(algo, meca);

  auto load = Loading::create();
  load->setTimeDiscretizationUser({1e-3, 2e-3, 1.e-2});
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++)
      load->setEvolution({i, j}, MechanicDriving::Stress, Evolution::Linear, 0.0);
  }
  load->setEvolution(Component::XX, MechanicDriving::Strain, Evolution::Linear, 0.01);
  auto lo = LoadingOutput::create();
  lo->add(load);

  auto input = Input::create(grid, algoPara, materials, lo);
  input->resultsDir = "testresults/simple_spheres_composites";
  input->generateFiles();

  std::filesystem::path refDir = "ref-amxdir/simple_spheres_composites";
  std::filesystem::path dir = input->resultsDir;

  // materialIds.vtk for 1 material is no longer generated
  // input->generateMaterialVTK(dir / "materialIds.vtk");
  //  zoneIds.vtk for 1 zone is no longer generated
  input->generateZoneVTK(dir / "zoneIds.vtk");

  EXPECT_TRUE(compareXMLFiles(dir / "materials.xml", refDir / "materials.xml"));
  EXPECT_TRUE(compareXMLFiles(dir / "algorithm.xml", refDir / "algorithm.xml"));
  EXPECT_TRUE(compareXMLFiles(dir / "loading.xml", refDir / "loading.xml"));

  EXPECT_TRUE(compareVtkWithRef(dir / "materialIds.vtk", refDir / "materialIds.vtk", 0));
  EXPECT_TRUE(compareVtkWithRef(dir / "zoneIds.vtk", refDir / "zoneIds.vtk", 0));

  EXPECT_TRUE(compareBinWithRef(dir / "composites" / "rep_1_2" / "fv1.bin",
                                refDir / "composites" / "rep_1_2" / "fv1.bin", 1.e-8));
  EXPECT_TRUE(compareBinWithRef(dir / "composites" / "rep_1_2" / "fv2.bin",
                                refDir / "composites" / "rep_1_2" / "fv2.bin", 1.e-8));
  EXPECT_TRUE(compareBinWithRef(dir / "composites" / "rep_1_2" / "pos.bin",
                                refDir / "composites" / "rep_1_2" / "pos.bin", 0.0));
  EXPECT_TRUE(compareBinWithRef(dir / "composites" / "rep_1_2" / "zone1.bin",
                                refDir / "composites" / "rep_1_2" / "zone1.bin", 0.0));
  EXPECT_TRUE(compareBinWithRef(dir / "composites" / "rep_1_2" / "zone2.bin",
                                refDir / "composites" / "rep_1_2" / "zone2.bin", 0.0));
  EXPECT_TRUE(compareBinWithRef(dir / "composites" / "rep_1_2" / "N12x.bin",
                                refDir / "composites" / "rep_1_2" / "N12x.bin", 1.e-8));
  EXPECT_TRUE(compareBinWithRef(dir / "composites" / "rep_1_2" / "N12y.bin",
                                refDir / "composites" / "rep_1_2" / "N12y.bin", 1.e-8));
  EXPECT_TRUE(compareBinWithRef(dir / "composites" / "rep_1_2" / "N12z.bin",
                                refDir / "composites" / "rep_1_2" / "N12z.bin", 1.e-8));
  for (size_t i = 0; i < input->materials->composite(0)->tangents().at(0).size(); i++) {
    for (size_t j = 0; j < input->materials->composite(0)->tangents().at(0).at(i).size(); j++) {
      Vector3D T, N;
      for (size_t k = 0; k < 3; k++)
        T[k] = input->materials->composite(0)->tangents().at(k).at(i).at(j);
      for (size_t k = 0; k < 3; k++)
        N[k] = input->materials->composite(0)->normals().at(k).at(i).at(j);
      double prod2 = 0.0;
      for (size_t k = 0; k < 3; k++) prod2 += T[k] * N[k];
      EXPECT_NEAR(prod2, 0.0, 1.e-8);
    }
  }

  std::ifstream fcompo{dir / "composites" / "list_composite_materials.txt"};
  ASSERT_TRUE(fcompo);
  std::string line;
  std::getline(fcompo, line);
  EXPECT_STREQ(line.c_str(), "1 2 laminate");
}

TEST(Composite, SimpleSpheres) {
  compositeSpheres(true);
  compositeSpheres(false);
}