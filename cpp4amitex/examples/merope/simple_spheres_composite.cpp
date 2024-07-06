#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "MultiInclusions/MultiInclusions.hxx"
#include "MultiInclusions/SphereInclusions.hxx"
#include "Voxellation/Voxellation.hxx"

#include "amitex/extract.hpp"
#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

using namespace merope;
using namespace sac_de_billes;

namespace amx = amitex;

void setParametersAlgorithm(amx::Input& input) {
  amx::Algorithm algo;
  algo.type = "Basic_Scheme";
  algo.convergenceAcceleration = true;
  algo.convergenceCriterion = 1.e-4;
  input.algorithmParameters.algorithm = algo;

  // No diffusion for composite (crash anyway)
  // amx::Diffusion diffu;
  // diffu.filter = "Default";
  // diffu.stationary = true;
  // input.algorithParameters.diffusion = diffu;

  amx::Mechanics meca;
  meca.filter = "Default";
  meca.smallPerturbations = true;  // not available with composites
  input.algorithmParameters.mechanics = meca;
}

void setLoading(amx::Input& input) {
  amx::Loading load;
  double times[] = {1};
  load.setTimeDiscretizationUser(1, times);
  for (int i = 0; i < 3; i++) {
    load.setEvolution(i, amx::DiffusionDriving::Gradient, amx::Evolution::Linear, 0.0);
  }

  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++)
      load.setEvolution({i, j}, amx::MechanicDriving::Strain, amx::Evolution::Linear,
                        i == 2 ? 1.0e-3 : 0.0);
  }
  load.setOutputVtkList({1});
  input.loadingOutput.add(std::move(load));
  input.loadingOutput.output.setVtkStressStrain(1, 1);
}

using Matrix3D = std::array<amx::Vector3D, 3>;

Matrix3D computeStress(vox::Voxellation<3>& voxelation, const std::vector<double>& coefficients,
                       const std::string& law = "reuss") {
  Matrix3D result;
  const auto& voxGrid = voxelation.getGrid();
  amx::GridSize n{voxGrid.getNx(), voxGrid.getNy(), voxGrid.getNz()};
  amx::Vector3D dx{{voxGrid.getLx() / voxGrid.getNx(), voxGrid.getLy() / voxGrid.getNy(),
                    voxGrid.getLz() / voxGrid.getNz()}};
  size_t totalSize = voxGrid.getNg();
  amx::Grid grid{n, dx};
  amx::Input input{grid};

  setParametersAlgorithm(input);
  setLoading(input);
  auto maxCoeff = *std::max_element(coefficients.begin(), coefficients.end());
  input.materials.referenceMaterialD = amx::ReferenceMaterialD{maxCoeff};

  input.materials.referenceMaterial = amx::ReferenceMaterial{maxCoeff, maxCoeff};

  input.materials.setNumberMaterials(coefficients.size());

  std::vector<amx::Zone> zonePures;
  for (auto _ : coefficients) zonePures.push_back(amx::Zone{grid.dims()});

  amx::Zone zoneInter{grid.dims()};
  std::map<std::vector<size_t>, size_t> compos;
  auto& composites = input.materials.composites;
  auto phases = voxelation.computePhaseGrid(grid.dims());
  std::cout << phases.size() << "\n";
  for (size_t p = 0; p < phases.size(); p++) {
    amx::GridPoint pos = voxGrid.get_coord_index<3>(p);
    amx::GridLinPoint lpos = grid.linearize(pos);
    using Phase = std::tuple<merope::vox::VTK_PHASE, double>;
    // Order of phase matte to avoid duplicates
    std::vector<Phase> phase = phases[p];
    std::sort(phase.begin(), phase.end(), [](Phase a, Phase b) { return get<0>(a) < get<0>(b); });
    std::vector<size_t> phaseIds;
    std::vector<double> volFracs;
    for (const auto& p : phase) {
      double vf = get<1>(p);
      if (vf > amx::Composite::maxVolumeFraction) {
        phaseIds.push_back(get<0>(p));
        volFracs.push_back(vf);
      }
    }
    if (phaseIds.size() == 0) throw amx::InputError{"phase volume fraction likely wrong"};
    if (phaseIds.size() == 1) {
      zonePures.at(get<0>(phases[p][0])).add(lpos);
    } else {
      zoneInter.add(pos);
      auto it = compos.find(phaseIds);
      if (it == compos.end()) {
        auto [it2, ok] = compos.insert({phaseIds, composites.numberMaterials()});
        if (!ok) throw amx::InputError{std::string{__func__} + " map could not inset"};
        composites.add(amx::Composite{phaseIds, law});
        it = it2;
      }
      size_t ic = it->second;
      if (ic >= composites.numberMaterials()) {
        std::cout << ic << " " << it->first.size() << "\n";
        for (const auto& p : phase) {
          std::cout << get<0>(p) << "\t" << get<1>(p) << "\n";
        }
      }
      composites.at(ic).addVoxel(lpos, volFracs);
    }
  }
  for (size_t m = 0; m < coefficients.size(); m++) {
    zonePures.emplace_back(grid.dims());
  }
  input.resultsDir = "amitex_merope_comp/";

  for (size_t m = 0; m < coefficients.size(); m++) {
    amx::Material material;
    material.setLawK("Fourier_iso_polarization");
    material.setLaw("elasiso");
    std::vector<double> coeffKs(4);
    coeffKs[0] = coefficients.at(m);
    for (size_t c = 1; c <= 3; c++) coeffKs[c] = 1 == c ? -coeffKs[0] : 0.0;
    material.setCoeffs({coefficients.at(m)});
    material.setCoeffComposites({coefficients.at(m)});
    material.setCoeffKs(coeffKs);  // Why ?
    material.addZone(zonePures[m]);
    material.addZone(zoneInter);  // Zone absent otherwise !?
    input.materials.material(m) = material;
  }

  runSimulationExternal(input);

  amx::Extract ext{input.outputPrefix()};
  // auto flux = ext.averageDiffusionFlux(0);
  auto sig = ext.averageStress();
  return sig;
}

int main() {
  auto sphIncl = SphereInclusions<3>();
  sphIncl.setLength({10, 10, 10});
  sphIncl.fromHisto(0, algoSpheres::TypeAlgo::RSA, 0.0, {{3, 0.5}}, {1});

  auto multiInclusions = MultiInclusions<3>();
  multiInclusions.setInclusions(sphIncl);
  auto grid = vox::Voxellation<3>(multiInclusions);
  grid.setPureCoeffs({1.0, 3.0});
  grid.setHomogRule(homogenization::Rule::Voigt);
  grid.setVoxelRule(vox::VoxelRule::Average);
  grid.proceed({32, 32, 32});

  auto sig = computeStress(grid, {1.0, 3.0}, "voigt");
  for (auto row : sig) {
    std::cout << row[0] << "\t" << row[1] << "\t" << row[2] << "\n";
  }
}
