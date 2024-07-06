#include "amitex/private/xml_utils.hpp"

namespace amitex {

template <>
void writeXMLAttributes(std::ostream& stream, std::string_view attr, const bool& value) {
  stream << " " << attr << "=\"" << (value ? "True" : "False") << "\"";
}

}  // namespace amitex
