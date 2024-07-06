#include "testutils.hpp"

#include <vector>

#include "amitex/errors.hpp"
#include "amitex/io.hpp"

namespace amitex_tests {

bool compareXMLFiles(const std::filesystem::path& fPath, const std::filesystem::path& refPath) {
  std::string cmd = "python3 compare_xml.py " + fPath.string() + " " + refPath.string();
  return system(cmd.c_str()) == 0;
}

static bool compareVectors(const std::vector<double>& data, const std::vector<double>& ref,
                           double tol) {
  if (data.size() != ref.size()) return false;
  for (size_t i = 0; i < data.size(); i++) {
    if (std::abs(data[i] - ref[i]) > tol * std::abs(ref[i])) {
      return false;
    }
  }
  return true;
}

bool compareVtkWithRef(const std::string& path, const std::string& ref, double tol) {
  std::vector<double> data, refdata;
  amitex::readVTK(path, data);
  if (data.empty()) throw amitex::InputError("problem with " + path);
  amitex::readVTK(ref, refdata);
  if (refdata.empty()) throw amitex::InputError("problem with " + ref);
  return compareVectors(data, refdata, tol);
}

bool compareBinWithRef(const std::string& path, const std::string& ref, double tol) {
  std::vector<double> data, refdata;
  amitex::readBin(path, data);
  amitex::readBin(ref, refdata);
  return compareVectors(data, refdata, tol);
}

}  // namespace amitex_tests
