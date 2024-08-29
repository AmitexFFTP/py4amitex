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

template <typename T, typename TDoc>
#if __cpp_concepts >= 201907L
  requires InputElement<T, TDoc>
#endif
void writeXML(std::ostream& stream, const T& obj) {
  TDoc doc{"DOC"};
  auto el = obj.toXMLElement(doc);
  doc.getRoot().appendChild(el);
  doc.write(stream, el);
}

template <typename T, typename TDoc>
#if __cpp_concepts >= 201907L
  requires InputElement<T, TDoc>
#endif
void writeXMLFileStream(std::ostream& file, const T& obj) {
  file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  TDoc doc{"DOC"};
  obj.toXMLElement(doc);
  writeXML(file, obj);
}

template <typename T, typename TDoc>
#if __cpp_concepts >= 201907L
  requires InputElement<T, TDoc>
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