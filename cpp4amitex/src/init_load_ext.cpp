#include "amitex/input/init_load_ext.hpp"

namespace amitex {

void InitLoadExt::xmlWriteInner(std::ostream& stream) const {
  if (temperature.value) writeXML(stream, temperature);
  for (const auto& param : params) writeXML(stream, param);
}

void InitLoadExt::setParam(size_t index, double value) {
  if (index >= params.size()) params.resize(index + 1);
  params[index].value = value;
  params[index].index = index;
}

void InitLoadExt::Param::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Index", index + 1);
  writeXMLAttributes(stream, "Value", value);
}

}  // namespace amitex