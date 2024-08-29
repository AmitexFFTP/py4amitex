#include "amitex/input/init_load_ext.hpp"

namespace amitex {

void InitLoadExt::setParam(size_t index, double value) {
  if (index >= params.size()) params.resize(index + 1);
  params[index].value = value;
  params[index].index = index;
}

}  // namespace amitex