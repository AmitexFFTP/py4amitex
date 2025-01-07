#include "amitex/io.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>

#include "amitex/errors.hpp"

#include "byteswap.h"

namespace amitex {

template <typename T>
size_t writeBEdata(FILE* stream, const T* data, size_t data_size) noexcept {
#if defined(AMITEX_BIG_ENDIAN)
  return fwrite(data, sizeof(T), data_size, stream);
#elif defined(AMITEX_LITTLE_ENDIAN)
  size_t ret = 0;
  for (size_t i = 0; i < data_size; i++) {
    T swapped = byteswap(data[i]);
    ret += fwrite(&swapped, sizeof(T), 1, stream);
  }
  return ret;
#else
  static_assert("Undefined endianness" && false);
#endif
}

template <typename T>
static int writeVTKHeader(FILE* stream, GridSize nx, Vector3D dx) noexcept {
  int ret;
  ret = fprintf(stream,
                "# vtk DataFile Version 4.5\n"
                "Materiau\n"
                "BINARY\n"
                "DATASET STRUCTURED_POINTS\n");
  if (ret > 0) ret = fprintf(stream, "DIMENSIONS %zd %zd %zd\n", nx[0] + 1, nx[1] + 1, nx[2] + 1);
  if (ret > 0) ret = fprintf(stream, "ORIGIN 0.000 0.000 0.000\n");
  if (ret > 0) ret = fprintf(stream, "SPACING %.7e %.7e %.7e\n", dx[0], dx[1], dx[2]);
  if (ret > 0) ret = fprintf(stream, "CELL_DATA %zd\n", nx[0] * nx[1] * nx[2]);
  if (ret > 0) {
    if constexpr (std::is_same_v<T, int>) {
      ret = fprintf(stream, "SCALARS MaterialId int\n");
    } else if constexpr (std::is_same_v<T, long>) {
      ret = fprintf(stream, "SCALARS MaterialId long\n");
    } else if constexpr (std::is_same_v<T, long long>) {
      ret = fprintf(stream, "SCALARS MaterialId long_long\n");
    } else if constexpr (std::is_same_v<T, double>) {
      ret = fprintf(stream, "SCALARS MaterialId double\n");
    }
  }
  if (ret > 0) ret = fprintf(stream, "LOOKUP_TABLE default\n");
  return ret;
}

template <typename T>
void writeVTKImpl(const std::filesystem::path& path, GridSize nx, Vector3D dx, const T* data,
                  size_t data_size) {
  FILE* stream = fopen(path.c_str(), "wb");
  if (stream == nullptr) throw InputError{path.string() + ": could not open file"};
  ;
  if (writeVTKHeader<T>(stream, nx, dx) < 0)
    throw InputError{path.string() + ": error writing VTK header"};

  size_t nwdata = writeBEdata(stream, data, data_size);
  if (nwdata != data_size) {
    fclose(stream);
    throw InputError{path.string() + ": could not write all data"};
  }
  fclose(stream);
}

void writeVTK(const std::filesystem::path& path, GridSize nx, Vector3D dx, const int* data,
              size_t dataSize) {
  writeVTKImpl(path, nx, dx, data, dataSize);
}

void writeVTK(const std::filesystem::path& path, GridSize nx, Vector3D dx, const long* data,
              size_t dataSize) {
  writeVTKImpl(path, nx, dx, data, dataSize);
}

void writeVTK(const std::filesystem::path& path, GridSize nx, Vector3D dx, const double* data,
              size_t dataSize) {
  writeVTKImpl(path, nx, dx, data, dataSize);
}

template <typename T>
static void writeBINImpl(const std::filesystem::path& path, const std::vector<T>& data) {
  FILE* stream = fopen(path.c_str(), "wb");
  if (stream == nullptr) throw InputError{"cannot open" + path.string()};
  fprintf(stream, "%zd\n", data.size());
  if constexpr (std::is_same_v<T, double>) {
    fprintf(stream, "double\n");
  } else if constexpr (std::is_same_v<T, unsigned long>) {
    fprintf(stream, "unsigned_long\n");
  } else {
    assert(false && "Unsupported BIN writer for this type");
  }
  size_t nwdata = writeBEdata(stream, data.data(), data.size());
  if (nwdata != data.size()) {
    throw InputError{path.string() + ": could not write all data"};
    fclose(stream);
  }

  fclose(stream);
}

void writeBIN(const std::filesystem::path& path, const std::vector<double>& data) {
  writeBINImpl(path, data);
}

void writeBIN(const std::filesystem::path& path, const std::vector<unsigned long>& data) {
  writeBINImpl(path, data);
}

template <typename Tin, typename Tout>
int readBEdata(FILE* stream, Tout* data, size_t data_size) noexcept {
  for (size_t i = 0; i < data_size; i++) {
    Tin swapped;
    if (fread(&swapped, sizeof(Tin), 1, stream) != 1) return 1;
#if defined(AMITEX_BIG_ENDIAN)
    data[i] = static_cast<Tout>(swapped);
#elif defined(AMITEX_LITTLE_ENDIAN)
    data[i] = static_cast<Tout>(byteswap(swapped));
#else
    static_assert("Undefined endianness" && false);
#endif
  }
  return 0;
}

static bool readVtkHeader(FILE* stream, VtkHeader& header) noexcept {
  constexpr size_t lineMax = 128;
  char headerStr[1200];
  size_t nread = fread(headerStr, 1, 1200, stream);
  size_t lineEnd = 0;
  char* line = headerStr;
  for (size_t l = 0; l < 10; l++) {
    while (lineEnd < nread && headerStr[lineEnd] != '\n') {
      lineEnd++;
    }
    if (lineEnd >= nread) return false;
    headerStr[lineEnd] = '\0';
    if (!strncmp(line, "DIMENSIONS", 10)) {
      long long nx, ny, nz;
      if (sscanf(line + 11, "%lld %lld %lld", &nx, &ny, &nz) != 3) return false;
      if (nx <= 0 || ny <= 0 || nz <= 0) return false;
      header.dimensions[0] = nx - 1;
      header.dimensions[1] = ny - 1;
      header.dimensions[2] = nz - 1;
    } else if (!strncmp(line, "ORIGIN", 6)) {
      if (sscanf(line + 7, "%lf %lf %lf", &header.origin[0], &header.origin[1],
                 &header.origin[2]) != 3)
        return false;
    } else if (!strncmp(line, "SPACING", 7)) {
      if (sscanf(line + 8, "%lf %lf %lf", &header.spacing[0], &header.spacing[1],
                 &header.spacing[2]) != 3)
        return false;
    } else if (!strncmp(line, "CELL_DATA", 9)) {
      if (sscanf(line + 10, "%zd", &header.cellDataSize) != 1) return false;
    } else if (!strncmp(line, "SCALARS", 7)) {
      char temp[64];
      if (sscanf(line + 8, "%*s %s", temp) != 1) return false;
      header.scalarType = temp;
    }
    lineEnd++;
    line = headerStr + lineEnd;
  }
  fseek(stream, lineEnd, SEEK_SET);
  return true;
}

template <typename T>
static int readBEDataFromType(FILE* stream, std::vector<T>& data, const std::string& type) {
  if (type == "char") {
    return readBEdata<char, T>(stream, data.data(), data.size());
  } else if (type == "unsigned_char") {
    return readBEdata<unsigned char, T>(stream, data.data(), data.size());
  } else if (type == "short") {
    return readBEdata<short, T>(stream, data.data(), data.size());
  } else if (type == "unsigned_short") {
    return readBEdata<unsigned short, T>(stream, data.data(), data.size());
  } else if (type == "int") {
    return readBEdata<int, T>(stream, data.data(), data.size());
  } else if (type == "unsigned_int") {
    return readBEdata<unsigned int, T>(stream, data.data(), data.size());
  } else if (type == "unsigned_long") {
    return readBEdata<unsigned long, T>(stream, data.data(), data.size());
  } else if (type == "long") {
    return readBEdata<long, T>(stream, data.data(), data.size());
  } else if (type == "unsigned_long") {
    return readBEdata<unsigned long, T>(stream, data.data(), data.size());
  } else if (type == "long_long") {
    return readBEdata<long long, T>(stream, data.data(), data.size());
  } else if (type == "unsigned_long_long") {
    return readBEdata<unsigned long long, T>(stream, data.data(), data.size());
  } else if (type == "double") {
    return readBEdata<double, T>(stream, data.data(), data.size());
  } else {
    return 2;
  }
}

template <typename T>
static VtkHeader readVTKImpl(const std::filesystem::path& path, std::vector<T>& data) {
  FILE* stream = fopen(path.c_str(), "rb");
  if (stream == nullptr) throw InputError("cannot open VTK file");
  VtkHeader header;
  if (!readVtkHeader(stream, header)) throw InputError("cannot parse VTK header");
  data.resize(header.cellDataSize);
  switch (readBEDataFromType(stream, data, header.scalarType)) {
    case 1:
      fclose(stream);
      throw InputError("Error reading VTK file data");
      break;
    case 2:
      fclose(stream);
      throw InputError("unsupported VTK scalar type");
    default:
      break;
  }
  fclose(stream);
  return header;
}

template <typename T>
static std::string readBinImpl(const std::filesystem::path& path, std::vector<T>& data) {
  FILE* stream = fopen(path.c_str(), "rb");
  if (stream == nullptr) throw InputError("cannot open BIN file");
  size_t size;
  if (fscanf(stream, "%zd\n", &size) != 1) throw InputError("cannot parse BIN header");
  char temp[64];
  if (fscanf(stream, "%s\n", temp) != 1) throw InputError("cannot parse BIN header");
  std::string scalarType = temp;
  data.resize(size);
  switch (readBEDataFromType<T>(stream, data, scalarType)) {
    case 1:
      fclose(stream);
      throw InputError("Error reading BIN file data");
      break;
    case 2:
      fclose(stream);
      throw InputError("unsupported BIN scalar type");
    default:
      break;
  }
  fclose(stream);
  return scalarType;
}

template <>
VtkHeader readVTK<int>(const std::filesystem::path& path, std::vector<int>& data) {
  return readVTKImpl<int>(path, data);
}

template <>
VtkHeader readVTK<long>(const std::filesystem::path& path, std::vector<long>& data) {
  return readVTKImpl<long>(path, data);
}

template <>
VtkHeader readVTK<long long>(const std::filesystem::path& path, std::vector<long long>& data) {
  return readVTKImpl<long long>(path, data);
}

template <>
VtkHeader readVTK<double>(const std::filesystem::path& path, std::vector<double>& data) {
  return readVTKImpl<double>(path, data);
}

std::string readBin(const std::filesystem::path& path, std::vector<double>& data) {
  return readBinImpl<double>(path, data);
}

}  // namespace amitex