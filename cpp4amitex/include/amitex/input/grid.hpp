#ifndef _AMITEX_GRID_HEADER_
#define _AMITEX_GRID_HEADER_

#include <cmath>

#include "amitex/input/common.hpp"

namespace amitex {

//! Unit cell geometry (sizes, lengths...)
class Grid {
 public:
  Grid() = default;
  Grid(GridSize dims, Vector3D dx) : dims_{dims}, voxelLengths_{dx} {}
  void setPbc(std::array<bool, 3> pbc) { pbc_ = pbc; }
  void setOrigin(Vector3D x0) { origin_ = x0; }
  //! Total number of voxels
  size_t totalSize() const { return dims_[0] * dims_[1] * dims_[2]; }
  //! Per-axis number of voxels
  GridSize dims() const { return dims_; }
  //! Lengths of a voxel
  Vector3D voxelLengths() const { return voxelLengths_; }

  //! Get global ranges of voxel coordinates
  std::pair<GridSize, GridSize> getPartition() const {
    GridSize ibegin, iend;
    ibegin = {0, 0, 0};
    iend = dims();
    return {ibegin, iend};
  }

  //! Get linearized position
  //! \param p position in grid coordinates
  GridLinPoint linearize(GridPoint p) const { return amitex::linearize(p, dims()); }

  //! Compute distance between two points
  double distance(const GridPoint& a, const GridPoint& b) const;
  double distance(const GridPoint& a, const Vector3D& b) const;

  class AllPoints {
   public:
    AllPoints(const GridSize& dims) : dims{dims} {}
    class Iterator {
     public:
      Iterator(const GridPoint& first, const GridSize& dims) : current{first}, dims{dims} {}
      Iterator& operator++() {
        current[0]++;
        if (current[0] >= dims[0]) {
          current[1]++;
          current[0] = 0;
          if (current[1] >= dims[1]) {
            current[2]++;
            current[1] = 0;
            current[0] = 0;
          }
        }
        return *this;
      }
      bool operator!=(const Iterator& it) const { return dims != it.dims || current != it.current; }
      bool operator==(const Iterator& it) const { return !(*this != it); }
      const GridPoint& operator*() const { return current; }

     protected:
      GridPoint current;
      GridSize dims;
    };
    Iterator begin() const { return Iterator{{0, 0, 0}, dims}; }
    Iterator end() const { return Iterator{{0, 0, dims[2]}, dims}; }

   private:
    GridSize dims;
  };

  //! \return an iterable over all points on the grid
  //! \example
  //! ```C++
  //! for(GridPoint p : grid.allPoints()) …
  //! ```
  AllPoints allPoints() const { return AllPoints{dims()}; }

  class AllLinPoints {
   public:
    AllLinPoints(const GridSize& dims) : dims{dims} {}
    class Iterator : public AllPoints::Iterator {
     public:
      Iterator(const GridPoint& first, const GridSize& dims) : AllPoints::Iterator{first, dims} {}
      GridLinPoint operator*() const { return amitex::linearize(current, dims); }
    };
    Iterator begin() const { return Iterator{{0, 0, 0}, dims}; }
    Iterator end() const { return Iterator{{0, 0, dims[2]}, dims}; }

   private:
    GridSize dims;
  };

  //! \return an iterable over all points (linear representation) on the grid
  //! \example
  //! ```C++
  //! for(GridLinPoint p : grid.allLinPoints()) …
  //! ```
  AllLinPoints allLinPoints() const { return AllLinPoints{dims()}; }

 private:
  GridSize dims_ = {0, 0, 0};
  Vector3D voxelLengths_ = {0, 0, 0};
  Vector3D origin_ = {0, 0, 0};
  std::array<bool, 3> pbc_ = {true, true, true};
};

}  // namespace amitex

#endif  // _AMITEX_GRID_HEADER_
