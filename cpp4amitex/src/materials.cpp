/*
SPDX-License-Identifier: LGPL-3.0-only

(C) copyright 2026 of CEA

author: Guillaume Bareigts <guillaume.bareigts@cea.fr>, CEA
*/
#include "amitex/input/materials.hpp"

namespace amitex {

void Materials::setNumberMaterials(size_t num) {
  materials.resize(num);
  for (auto& mat : materials) {
    if (!mat) mat = Material::create();
  }
}

}  // namespace amitex