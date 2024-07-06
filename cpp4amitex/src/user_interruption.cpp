#include "amitex/input/user_interruption.hpp"
#include "amitex/private/xml_utils.hpp"

namespace amitex {

void UserInterruption::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Index", index_);
  writeXMLAttributes(stream, "Value", value_);
}

}  // namespace amitex
