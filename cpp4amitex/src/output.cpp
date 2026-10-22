/*
SPDX-License-Identifier: LGPL-3.0-only

(C) copyright 2026 of CEA

author: Guillaume Bareigts <guillaume.bareigts@cea.fr>, CEA
*/
#include <amitex/input/output.hpp>

namespace amitex {

void Output::Zone::setVarIntList(const std::vector<size_t>& list) {
  for (auto var : list) varIntList.add(var + 1);
}

}  // namespace amitex
