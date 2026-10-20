#include "amitex/input/materials.hpp"

namespace amitex {

void Materials::setNumberMaterials(size_t num) {
  materials.resize(num);
  for (auto& mat : materials) {
    if (!mat) mat = Material::create();
  }
}

}  // namespace amitex