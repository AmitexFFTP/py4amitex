#ifndef _AMITEX_TESTUTILS_HEADER_
#define _AMITEX_TESTUTILS_HEADER_

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "amitex/private/xml_document.hpp"

namespace amitex_tests {

template <typename T>
void writeXML(std::ostream& stream, const T& obj) {
  amitex::xml::Document doc{"DOC"};
  auto el = obj.toXMLElement(doc);
  doc.getRoot().appendChild(el);
  doc.write(stream, el);
}

template <typename T>
void writeXMLFileStream(std::ostream& file, const T& obj) {
  file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  amitex::xml::Document doc{"DOC"};
  obj.toXMLElement(doc);
  writeXML(file, obj);
}

template <typename T>
void writeXMLFile(const std::filesystem::path& path, const T& obj) {
  std::ofstream file{path};
  if (file) {
    writeXMLFileStream(file, obj);
  } else {
    throw std::runtime_error(std::string{"Could not open file"} + path.string());
  }
}

template <class T>
bool compareFilesExact(const T& obj, const std::filesystem::path& refPath) {
  std::ostringstream sstream;
  writeXMLFileStream(sstream, obj);

  std::ifstream refFile{refPath};
  std::ostringstream refSstream;
  refSstream << refFile.rdbuf();
  if (refFile.bad()) return false;

  bool ret = sstream.str() == refSstream.str();
  if (!ret) std::cerr << refSstream.str() << " != '" << sstream.str() << "'\n";
  return ret;
}

template <class T>
bool compareXMLWithRef(const T& obj, const std::filesystem::path& path) {
  std::filesystem::create_directories("testresults/xml");
  std::filesystem::path fPath = "testresults/xml" / path;
  std::filesystem::path refPath = "ref-xml" / path;
  obj.template toXMLDocument<amitex::xml::Document>().writeFile(fPath);
  std::string cmd = "python3 compare_xml.py " + fPath.string() + " " + refPath.string();
  return system(cmd.c_str()) == 0;
}

bool compareXMLFiles(const std::filesystem::path& fPath, const std::filesystem::path& refPath);

template <class T>
std::string toXMLString(const T& obj) {
  std::ostringstream sstream;
  writeXML(sstream, obj);
  return sstream.str();
}

//! Like `toXMLString` but for classes that generate XML documents (AlgorithmParameters, …)
template <class T>
std::string toXMLStringDoc(const T& obj) {
  std::ostringstream sstream;
  obj.template toXMLDocument<amitex::xml::Document>().write(sstream);
  return sstream.str();
}

bool compareVtkWithRef(const std::string& path, const std::string& ref, double tol);
bool compareBinWithRef(const std::string& path, const std::string& ref, double tol);

bool compareFileWithRef(const std::string& path, const std::string& ref);

//! default floating-point precision for comparison
constexpr double eps = 1.0e-6;

}  // namespace amitex_tests

#endif  // _AMITEX_TESTUTILS_HEADER_