#ifndef _AMITEX_COMMON_HEADER_
#define _AMITEX_COMMON_HEADER_

#include <array>
#include <cstdint>

namespace amitex {

using std::size_t;

//! Type of the dimensions of a grid or range in a grid
using GridSize = std::array<size_t, 3>;
//! Type of  the coordinates on a grid
using GridPoint = std::array<size_t, 3>;
//! Type of the linearized (leading order Z,Y,Z position on a grid
using GridLinPoint = size_t;

//! 3D vector
using Vector3D = std::array<double, 3>;
//! 3D 2-rank Tensor indexed in the following order: XX YY ZZ XY XZ YZ YZ ZX ZY
using Tensor3D = std::array<std::array<double, 3>, 3>;

//! Convert to a linearized position
//! \param p position
//! \param dims grid dimensions
//! \return linear position (leading dimension is Z, then Y, X)
inline GridLinPoint linearize(GridPoint p, GridSize dims) {
  return p[0] + dims[0] * (p[1] + dims[1] * p[2]);
}

}  // namespace amitex

#endif  // _AMITEX_COMMON_HEADER_
