#include "amitex/input/intvar.hpp"

namespace amitex {

void IntVar::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Index", id + 1);
  if (!field.empty()) {
    writeXMLAttributes(stream, "Type", "Variable");
    writeXMLAttributes(stream, "File", file);
    writeXMLAttributes(stream, "Format", "vtk");
  } else if (values.size() <= 1) {
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
