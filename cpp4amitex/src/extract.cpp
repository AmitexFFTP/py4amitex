#include "amitex/extract.hpp"

#include <fstream>
namespace amitex {

void Extract::setSymComponents(TensorLin& values) {
  size_t size = 6;
  if (size == 6) {
    for (size_t i = 6; i < 9; i++) {
      values[i] = values[i - 3];
    }
  }
}

void Extract::readStd(MechanicalQuantities& q) {
  std::string path = prefix + ".std";
  auto last = std::filesystem::last_write_time(path);
  if (lastWrite < last) {
    lastWrite = last;
    return;
  }
  std::ifstream file{path};
  std::string line;
  while ((getline(file, line), line.size() > 0 && line[0] == '#')) {
  }
  while (!file.eof()) {
    file >> q.t;
    for (size_t i = 0; i < 6; i++) file >> q.stress[i];
    for (size_t i = 0; i < 6; i++) file >> q.strain[i];
    for (size_t i = 0; i < 6; i++) file >> q.sigStress[i];
    for (size_t i = 0; i < 6; i++) file >> q.sigStrain[i];
    file >> q.niter;
    if (file.bad()) throw InputError{"Extract: Error in " + path};
  }
}

Tensor3D Extract::toTensor3D(const TensorLin& tensor) {
  Tensor3D result;

  for (size_t i = 0; i < 3; i++) result[i][i] = tensor[i];
  result[0][1] = tensor[3];
  result[0][2] = tensor[4];
  result[1][2] = tensor[5];
  result[1][0] = tensor[6];
  result[2][0] = tensor[7];
  result[2][1] = tensor[8];
  return result;
}

Tensor3D Extract::averageStress() {
  readStd(currentMechanics);
  setSymComponents(currentMechanics.stress);
  return toTensor3D(currentMechanics.stress);
}

Tensor3D Extract::averageStrain() {
  readStd(currentMechanics);
  setSymComponents(currentMechanics.strain);
  return toTensor3D(currentMechanics.strain);
}

void Extract::readStd(DiffusionQuantities& q) {
  std::string path = prefix + ".std";
  auto last = std::filesystem::last_write_time(path);
  if (lastWrite < last) {
    lastWrite = last;
    return;
  }
  std::ifstream file{path};
  std::string line;
  while ((getline(file, line), line.size() > 0 && line[0] == '#')) {
  }
  while (!file.eof()) {
    file >> q.t;
    for (size_t i = 0; i < 3; i++) file >> q.flux[i];
    for (size_t i = 0; i < 3; i++) file >> q.gradient[i];
    for (size_t i = 0; i < 3; i++) file >> q.sigFlux[i];
    for (size_t i = 0; i < 3; i++) file >> q.sigGradient[i];
    file >> q.niter;
    if (file.bad()) throw InputError{"Extract: Error in " + path};
  }
}

Vector3D Extract::averageDiffusionGradient(int indexD) {
  Vector3D values;
  readStd(currentDiffusion);
  return currentDiffusion.gradient;
}
Vector3D Extract::averageDiffusionFlux(int indexD) {
  Vector3D values;
  readStd(currentDiffusion);
  return currentDiffusion.flux;
}

}  // namespace amitex
