#ifndef _AMITEX_FIELD_HEADER_
#define _AMITEX_FIELD_HEADER_

#include <sstream>
#include <stdexcept>
#include <vector>

#include "amitex/input/common.hpp"
#include "amitex/io.hpp"

//! \file field.hpp
//! Defined 3D fields that may be defined in a rectangluar region of space
//! (e.g. for MPI partitioning)
//!
//! Optional bound-checked accessor (x(i,j,k) x[{i,j,k}]) is off when NDEBUG is defined

namespace amitex {

//! Field template class
//!
//! The data_ is indexed by Fortran convention (ie left-most index is first in memory,
template <typename T = double>
class Field {
 public:
  Field() = default;
  //! Field on the whole grid
  //! \param gridDims grid dimensions
  Field(GridSize gridDims)
      : nx{gridDims},
        data_(gridDims[0] * gridDims[1] * gridDims[2]),
        ibegin{0, 0, 0},
        iend{gridDims} {
    dataPtr_ = data_.data();
  }
  //! Field on the whole grid, non-owning memory
  //! \param gridDims grid dimensions
  //! \param buffer
  Field(GridSize gridDims, T* dataPtr)
      : nx{gridDims},
        data_(gridDims[0] * gridDims[1] * gridDims[2]),
        ibegin{0, 0, 0},
        iend{gridDims},
        dataPtr_{dataPtr} {}
  //! Field on a rectangular region
  //! \param gridDims grid dimensions
  //! \param ibegin beginning of the rane in grid coordinates (*inclusive*)
  //! \param iend end of the rane in grid coordinates (*exclusive*)
  Field(GridSize gridDims, GridSize ibegin, GridSize iend)
      : nx{gridDims}, data_(gridDims[0] * gridDims[1] * gridDims[2]), ibegin{ibegin}, iend{iend} {
    dataPtr_ = data_.data();
  }

  //! Get dimensions
  std::array<size_t, 3> dims() const { return nx; }

  //! Get total size
  size_t size() const { return nx[0] * nx[1] * nx[2]; }

  //! get data_ at grid point without bound checking
  const T& uncheckedAt(size_t ix, size_t iy, size_t iz) const {
    size_t idx =
        (ix - ibegin[0]) +
        (iend[0] - ibegin[0]) * ((iy - ibegin[1]) + (iend[1] - ibegin[1]) * (iz - ibegin[2]));
    return data_[idx];
  }
  T& uncheckedAt(size_t ix, size_t iy, size_t iz) {
    size_t idx =
        (ix - ibegin[0]) +
        (iend[0] - ibegin[0]) * ((iy - ibegin[1]) + (iend[1] - ibegin[1]) * (iz - ibegin[2]));
    return dataPtr_[idx];
  }
  //! get data_ at grid point with optional bound checking
  const T& operator()(size_t ix, size_t iy, size_t iz) const {
#ifndef NDEBUG
    checkBounds({ix, iy, iz});
#endif
    return uncheckedAt(ix, iy, iz);
  }
  T& operator()(size_t ix, size_t iy, size_t iz) {
#ifndef NDEBUG
    checkBounds({ix, iy, iz});
#endif
    return uncheckedAt(ix, iy, iz);
  }
  //! get data_ at grid point with optional bound checking
  //! \param p grid coordinates
  T& operator[](GridPoint p) {
#ifndef NDEBUG
    checkBounds(p);
#endif
    return uncheckedAt(p[0], p[1], p[2]);
  }
  const T& operator[](GridPoint p) const {
#ifndef NDEBUG
    checkBounds(p);
#endif
    return uncheckedAt(p[0], p[1], p[2]);
  }
  //! get data_ at grid point with bound checking
  const T& at(size_t ix, size_t iy, size_t iz) const {
    checkBounds({ix, iy, iz});
    return uncheckedAt(ix, iy, iz);
  }
  T& at(size_t ix, size_t iy, size_t iz) {
    checkBounds({ix, iy, iz});
    return uncheckedAt(ix, iy, iz);
  }
  //! Fill all the (owned) space with `value`.
  void fill(const T& value) {
    for (size_t i = 0; i < data_.size(); i++) {
      dataPtr_[i] = value;
    };
  }
  //! get underlying data buffer
  T* data() { return dataPtr_; }
  const T* data() const { return dataPtr_; }
  //! Get range [begin, end) of accessible coordinates
  std::pair<GridSize, GridSize> bounds() { return {ibegin, iend}; }
  //! Get lower bound (inclusive)
  size_t lbound(size_t dim) { return ibegin[dim]; }
  //! Get upper bound (exclusive)
  size_t ubound(size_t dim) { return iend[dim]; }
  //! Check if `p` is in bounds
  bool inBounds(GridPoint p) {
    for (size_t d = 0; d < p.size(); d++) {
      if ((p[d] < ibegin[d]) || (p[d] >= iend[d])) return false;
    }
    return true;
  }
  //! Check if {`ix`, `iy`, `iz`} is in bounds
  bool inBounds(size_t ix, size_t iy, size_t iz) { return inBounds({ix, iy, iz}); }

  //! check if no data allocated
  bool empty() const { return size() == 0; }

  //! Create a new instance that shares memory buffer with the original
  //! \warning
  //! The validity of the field data is not check (same as the general case of taking a pointer to
  //! data)
  Field shallowCopy() const {
    Field view;
    view.dataPtr_ = dataPtr_;
    view.nx = nx;
    view.ibegin = ibegin;
    view.iend = iend;
    return view;
  }
  //! Create a field from a TVK file
  //! \param path file path
  static Field<T> loadFromVtk(const std::filesystem::path& path) {
    std::vector<T> data;
    auto header = readVTK(path, data);
    Field<T> ret;
    ret.nx = header.dimensions;
    ret.iend = ret.nx;
    ret.data_ = std::move(data);
    ret.dataPtr_ = &ret.data_[0];
    return ret;
  }

 private:
  void checkBounds(GridPoint p) {
    for (size_t d = 0; d < p.size(); d++) {
      if ((p[d] < ibegin[d]) || (p[d] >= iend[d])) {
        std::stringstream message;
        message << "Field index " << p[d] << " (at dim " << d << ") is out of range [" << ibegin[d]
                << "," << iend[d] << ")";
        throw std::out_of_range(message.str());
      }
    }
  }
  T* dataPtr_ = nullptr;
  std::vector<T> data_;
  GridSize nx = {0, 0, 0};  // global grid
  // local partition
  GridSize ibegin = {0, 0, 0};
  GridSize iend = {0, 0, 0};
};

}  // namespace amitex

#endif  // _AMITEX_FIELD_HEADER_
