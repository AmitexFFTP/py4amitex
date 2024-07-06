#include "amitex/private/basic_coeff.hpp"

namespace amitex {

void BasicCoeff::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Index", id + 1);
  if (values.size() <= 1) {
    double value = values.size() == 0 ? constantValue() : values[0];
    writeXMLAttributes(stream, "Type", "Constant");
    writeXMLAttributes(stream, "Value", value);
  } else {
    writeXMLAttributes(stream, "Type", "Constant_Zone");
    writeXMLAttributes(stream, "File", file);
    writeXMLAttributes(stream, "Format", "Binary");
  }
  if (!name_.empty()) writeXMLAttributes(stream, "Name", name_);
}

}  // namespace amitex