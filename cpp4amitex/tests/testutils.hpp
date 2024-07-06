#ifndef _AMITEX_TESTUTILS_HEADER_
#define _AMITEX_TESTUTILS_HEADER_

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "amitex/private/xml_utils.hpp"

namespace amitex_tests {

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
  writeXMLFile(fPath, obj);
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

bool compareVtkWithRef(const std::string& path, const std::string& ref, double tol);
bool compareBinWithRef(const std::string& path, const std::string& ref, double tol);

//! default floating-point precision for comparison
constexpr double eps = 1.0e-6;

}  // namespace amitex_tests

#endif  // _AMITEX_TESTUTILS_HEADER_