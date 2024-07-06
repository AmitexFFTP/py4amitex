#ifndef _AMITEX_IO_HEADER_
#define _AMITEX_IO_HEADER_

#include <filesystem>
#include <vector>

#include "amitex/input/common.hpp"

namespace amitex {

//! write a scalar-field data to a legacy VTK file
//! \param path file to write
//! \param nx grid dimensions
//! \param dx lengths of a voxel
//! \param data data buffer
//! \param data_size number of elements
void writeVTK(const std::filesystem::path& path, GridSize nx, Vector3D dx, const int* data,
              size_t data_size);
void writeVTK(const std::filesystem::path& path, GridSize nx, Vector3D dx, const long* data,
              size_t data_size);
void writeVTK(const std::filesystem::path& path, GridSize nx, Vector3D dx, const long long* data,
              size_t data_size);
void writeVTK(const std::filesystem::path& path, GridSize nx, Vector3D dx, const double* data,
              size_t data_size);

//! Write an AMITEX BIN file
void writeBIN(const std::filesystem::path& path, const std::vector<double>& data);
void writeBIN(const std::filesystem::path& path, const std::vector<unsigned long>& data);

//! Information contained in the header of a VTK file
struct VtkHeader {
  std::array<size_t, 3> dimensions;  //! grid dimensions
  std::array<double, 3> spacing;     //! grid dimensions
  std::array<double, 3> origin;      //! grid origin
  std::string scalarType;            //! scalar type
  size_t cellDataSize;               //! cell data size
};

//! Read VTK file
//! \param path path to file
//! \param[out] data data vector (in VTK voxel order)
//! \return header data
//! \throws std::runtime_error on input error or file not readable
//! \note only defined for (unsigned) int, (unsigned) short, (unsigned) long, (unsigned) long long
//! and double
template <typename T>
VtkHeader readVTK(const std::filesystem::path& path, std::vector<T>& data);

//! Read BIN file
//! \param path path to file
//! \param[out] data data vector (in VTK voxel order)
//! \return type of data in filed
//! \throws std::runtime_error on input error or file not
std::string readBin(const std::filesystem::path& path, std::vector<double>& data);

}  // namespace amitex

#endif  // _AMITEX_IO_HEADER_