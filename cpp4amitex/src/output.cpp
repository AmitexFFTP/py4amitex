#include <amitex/input/output.hpp>

namespace amitex {

void Output::Zone::setVarIntList(const std::vector<size_t>& list) {
  for (auto var : list) varIntList.add(var + 1);
}

}  // namespace amitex
