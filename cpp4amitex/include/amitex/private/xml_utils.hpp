#ifndef _AMITEX__XML_UTILS_HEADER_
#define _AMITEX__XML_UTILS_HEADER_

#include <filesystem>
#include <fstream>
#include <ostream>
#include <string_view>

#include "amitex/private/input_element.hpp"

//! \file xml_utils.hpp
//! Helper functions for XML generation

namespace amitex {

template <typename T>
void writeXMLAttributes(std::ostream& stream, std::string_view attr, const T& value) {
  stream << " " << attr << "=\"" << value << "\"";
}

template <>
void writeXMLAttributes(std::ostream& stream, std::string_view attr, const bool& value);

template <typename T>
#if __cpp_concepts >= 201907L
  requires InputElement<T>
#endif
void writeXML(std::ostream& stream, const T& value) {
  stream << "<" << value.xmlTag();
  value.xmlWriteAttributes(stream);
  if (value.xmlHasBody()) {
    stream << ">";
    value.xmlWriteInner(stream);
    stream << "</" << value.xmlTag() << ">";
  } else {
    stream << "/>";
  }
}

template <typename T>
#if __cpp_concepts >= 201907L
  requires InputElement<T>
#endif
void writeXMLFileStream(std::ostream& file, const T& obj) {
  file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  writeXML(file, obj);
}

template <typename T>
#if __cpp_concepts >= 201907L
  requires InputElement<T>
#endif
void writeXMLFile(const std::filesystem::path& path, const T& obj) {
  std::ofstream file{path};
  if (file) {
    writeXMLFileStream(file, obj);
  } else {
    throw std::runtime_error(std::string{"Could not open file"} + path.string());
  }
}

}  // namespace amitex

#endif  // _AMITEX__XML_UTILS_HEADER_