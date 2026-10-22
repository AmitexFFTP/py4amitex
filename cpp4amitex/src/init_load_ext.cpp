/*
SPDX-License-Identifier: LGPL-3.0-only

(C) copyright 2026 of CEA

author: Guillaume Bareigts <guillaume.bareigts@cea.fr>, CEA
*/
#include "amitex/input/init_load_ext.hpp"

namespace amitex {

void InitLoadExt::setParam(size_t index, double value) {
  if (index >= params.size()) params.resize(index + 1);
  params[index].value = value;
  params[index].index = index;
}

}  // namespace amitex