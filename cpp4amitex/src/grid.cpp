#include "amitex/input/grid.hpp"

namespace amitex {

double Grid::distance(const GridPoint& a, const GridPoint& b) const {
  using SComp = std::ptrdiff_t;
  double sep2 = 0.0;
  for (size_t i = 0; i < 3; i++) {
    SComp sep = static_cast<SComp>(a[i]) - static_cast<SComp>(b[i]);
    sep2 += (sep * voxelLengths()[i]) * (sep * voxelLengths()[i]);
  }
  return sqrt(sep2);
}

double Grid::distance(const GridPoint& a, const Vector3D& b) const {
  using SComp = std::ptrdiff_t;
  double sep2 = 0.0;
  for (size_t i = 0; i < 3; i++) {
    double sep = a[i] * voxelLengths()[i] - b[i];
    sep2 += sep * sep;
  }
  return sqrt(sep2);
}

}  // namespace amitex
