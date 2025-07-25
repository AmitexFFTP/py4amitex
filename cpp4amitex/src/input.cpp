#include "amitex/input.hpp"

#include <algorithm>
#include <filesystem>

#include "amitex/errors.hpp"
#include "amitex/io.hpp"

namespace amitex {

template <typename T>
static void handleZoneCoeffs(T& coeff, const std::string& resultsDir, size_t m, size_t c) {
  coeff.setIndex(c);
  auto path = resultsDir + "/" + coeff.xmlTag() + std::to_string(m + 1) + "_" +
              std::to_string(c + 1) + ".bin";
  coeff.setFile(path);
  if (coeff.zoneValues().size() > 0) {
    writeBIN(path, coeff.zoneValues());
  }
}

template <typename T>
static bool checkZoneCovered(const T* data, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (data[i] == -1) return false;
  }
  return true;
}

void handleIntVars(Material& mat, size_t m, const std::string& resultsDir, const Grid& grid) {
  for (size_t i = 0; i < mat.numberIntVars(); i++) {
    IntVar& ivar = mat.intVar(i);
    if (!ivar.field.empty()) {
      const std::string path =
          resultsDir + "/intvar_" + std::to_string(m + 1) + "_" + std::to_string(i + 1) + ".vtk";
      writeVTK(path, grid.dims(), grid.voxelLengths(), ivar.field.data(), grid.totalSize());
      ivar.setFile(path);
    } else if (!ivar.zoneValues().empty()) {
      const std::string path =
          resultsDir + "/intvar_" + std::to_string(m + 1) + "_" + std::to_string(i + 1) + ".bin";
      writeBIN(path, ivar.zoneValues());
      ivar.setFile(path);
    }
  }
}

static void generateComposite(Materials& materials, const std::string& path);

void Input::generateFiles() {
  std::filesystem::create_directories(resultsDir + "/output");
  writeXMLFile(algorithmPath(), *algorithmParameters);

  if (algorithmParameters->mechanics && !loadingOutput->output->vtkStressStrain)
    loadingOutput->output->vtkStressStrain = VtkStressStrain::create(0, 0);
  if (algorithmParameters->diffusion && !loadingOutput->output->vtkFluxDGradD)
    loadingOutput->output->vtkFluxDGradD = VtkFluxDGradD::create(0, 0);
  writeXMLFile(loadingPath(), *loadingOutput);

  if (grid.totalSize() == 0) throw InputError{"Input::generateFiles(): grid not initialized"};
  std::vector<int32_t> numM(grid.totalSize());
  std::vector<int64_t> numZ(grid.totalSize());
  fill(numM.begin(), numM.end(), -1);
  fill(numZ.begin(), numZ.end(), -1);
  generateComposite(*materials, resultsDir + "/composites");
  for (size_t m = 0; m < materials->numberMaterials(); m++) {
    Material& mat = *materials->material(m);
    mat.setIndex(m);
    for (size_t c = 0; c < mat.numberCoeff(); c++) {
      handleZoneCoeffs(mat.coeff(c), resultsDir, m, c);
    }
    for (size_t c = 0; c < mat.numberCoeffK(); c++) {
      handleZoneCoeffs(mat.coeffK(c), resultsDir, m, c);
    }
    for (size_t c = 0; c < mat.numberCoeffComposite(); c++) {
      handleZoneCoeffs(mat.coeffComposite(c), resultsDir, m, c);
    }
    size_t iz = 1;
    for (auto zone : mat.zones()) {
      for (auto pos : zone->linearPositions()) {
        if (pos >= numM.size())
          throw InputError{"Input::generateFiles(): zone has voxels outside of grid"};
        numM[pos] = m + 1;
        numZ[pos] = iz;
      }
      iz++;
    }
    handleIntVars(mat, m, resultsDir, grid);
  }
  if (!checkZoneCovered(numM.data(), grid.totalSize()))
    throw InputError{"Not all voxels are covered by a material"};
  if (!checkZoneCovered(numZ.data(), grid.totalSize()))
    throw InputError{"Not all voxels are covered by a zone"};
  writeXMLFile(materialsPath(), *materials);

  writeVTK(materialIdsPath(), grid.dims(), grid.voxelLengths(), numM.data(), grid.totalSize());
  writeVTK(zoneIdsPath(), grid.dims(), grid.voxelLengths(), numZ.data(), grid.totalSize());

  std::ofstream cmds{resultsDir + "/commands.in"};

  cmds << "&CMD\n";
  cmds << "fic_numM=\"" << materialIdsPath() << "\"\n";
  cmds << "fic_numZ=\"" << zoneIdsPath() << "\"\n";
  cmds << "fic_mat=\"" << materialsPath() << "\"\n";
  cmds << "fic_char=\"" << loadingPath() << "\"\n";
  cmds << "fic_algo=\"" << algorithmPath() << "\"\n";
  cmds << "fic_vtk=\"" << outputPrefix() << "\"\n";
  cmds << "/\n";
}

static void genCompositeZone(Composite& mat, const Materials& materials, const std::string& dir,
                             std::optional<Interphase>& interphase) {
  size_t nphases = mat.numberPhases();
  for (size_t i = 0; i < nphases; i++) {
    std::vector<size_t> zones(mat.zone(i).size());
    for (size_t p = 0; p < zones.size(); p++) zones[p] = mat.zone(i)[p] + 1;
    writeBIN(dir + "/zone" + std::to_string(i + 1) + ".bin", zones);
  }
}

static void genInterphase(Materials& materials) {
  if (materials.numberComposites() == 0) return;
  auto& interphase = materials.interphase;
  size_t nphases = 0;
  size_t pmin = 0, pmax = 0;
  for (size_t c = 0; c < materials.numberComposites(); c++) {
    Composite& mat = *materials.composite(c);
    nphases = std::max(nphases, mat.numberPhases());
    const auto& matPos = mat.positions();
    auto [matpmin, matpmax] = std::minmax_element(matPos.begin(), matPos.end());
    pmin = std::min(*matpmin, pmin);
    pmax = std::max(*matpmax, pmax);
  }
  // Construct dictionary  position -> is composite
  std::vector<bool> isComposite(pmax - pmin + 1);
  std::fill(isComposite.begin(), isComposite.end(), false);
  for (size_t c = 0; c < materials.numberComposites(); c++) {
    Composite& mat = *materials.composite(c);
    for (size_t p : mat.positions()) {
      isComposite[p - pmin] = true;
    }
  }

  for (size_t m = 0; m < materials.numberMaterials(); m++) {
    const Material& pureMat = *materials.material(m);
    std::vector<std::size_t> coveredZones;  // zones fully covered by a composite
    size_t izone = 1;
    for (auto zone : pureMat.zones()) {
      size_t coveredPos = 0;
      for (auto lpos : zone->linearPositions()) {
        if (lpos >= pmin && lpos <= pmax) {
          if (isComposite.at(lpos - pmin)) {
            coveredPos++;
          }
        }
      }
      if (coveredPos == zone->numberVoxels()) {
        coveredZones.push_back(izone);
      }
      izone++;
    }
    if (coveredZones.size() > 0 || pureMat.numberZones() == 0) {
      if (!interphase) interphase = Interphase{};
      if (coveredZones.size() == pureMat.numberZones())
        interphase.value().addMaterial(m, pureMat.numberZones());
      else
        interphase.value().addZones(m, coveredZones);
    }
  }
}

static const char* axisStr[] = {"x", "y", "z"};

static void genCompositeInterVec(size_t nphases,
                                 const std::array<std::vector<std::vector<double>>, 3>& vec,
                                 const std::string& prefix) {
  for (size_t k = 0; k < 3; k++) {
    size_t idx = 0;
    for (size_t i = 0; i < nphases; i++) {
      for (size_t j = i + 1; j < nphases; j++) {
        std::ostringstream path;
        path << prefix << i + 1 << j + 1 << axisStr[k] << ".bin";
        writeBIN(path.str(), vec[k][idx]);
      }
      idx++;
    }
  }
}

static void genCompositeTangent(size_t nphases,
                                const std::array<std::vector<std::vector<double>>, 3>& vec,
                                const std::string& prefix) {
  // Only one interface tangent is asked for the 'laminate' rule
  for (size_t k = 0; k < 3; k++) {
    std::ostringstream path;
    path << prefix << axisStr[k] << ".bin";
    writeBIN(path.str(), vec[k][0]);
  }
}

static void generateComposite(Materials& materials, const std::string& path) {
  materials.composites.setDirectory(path);
  std::filesystem::create_directories(path);
  std::ofstream fdef{path + "/list_composite_materials.txt"};
  if (!fdef) throw InputError{"could not open " + path + "/list_composite_materials.txt"};
  for (size_t m = 0; m < materials.numberComposites(); m++) {
    Composite& mat = *materials.composite(m);
    for (auto matId : mat.materialIndices()) fdef << matId + 1 << " ";
    fdef << mat.law() << "\n";
    std::ostringstream dir_;
    dir_ << "rep";
    for (auto matId : mat.materialIndices()) dir_ << "_" << matId + 1;
    std::string dir = path + "/" + dir_.str();
    std::filesystem::create_directories(dir);
    std::vector<GridLinPoint> opos(mat.positions().size());
    for (size_t i = 0; i < opos.size(); i++) opos[i] = mat.positions()[i] + 1;
    writeBIN(dir + "/pos.bin", opos);
    size_t nphases = mat.numberPhases();
    for (size_t i = 0; i < nphases; i++) {
      writeBIN(dir + "/fv" + std::to_string(i + 1) + ".bin", mat.volumeFractions(i));
    }
    genCompositeZone(mat, materials, dir, materials.interphase);
    genCompositeInterVec(nphases, mat.normals(), dir + "/N");
    genCompositeTangent(nphases, mat.tangents(), dir + "/T");
    size_t idx = 0;
    for (size_t i = 0; i < nphases; i++) {
      for (size_t j = i + 1; j < nphases; j++) {
        writeBIN(dir + "/S" + std::to_string(i + 1) + std::to_string(j + 1) + ".bin",
                 mat.surfaces()[i]);
      }
      idx++;
    }
  }
  genInterphase(materials);
}

}  // namespace amitex
