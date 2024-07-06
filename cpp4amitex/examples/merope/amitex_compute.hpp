#ifndef _AMITEX_COMPUTE
#define _AMITEX_COMPUTE

#include <array>
#include <cstdint>
#include <vector>

#include "Voxellation/Voxellation.hxx"
#include "amitex/input.hpp"

namespace merope {
namespace amx {

using namespace amitex;

template <unsigned short DIM>
using Matrix = std::array<std::array<double, DIM>, DIM>;

Matrix<3> computeThermalCoeff(const Grid_VER& grid, const std::vector<double>& coefficients,
                              int nbProcs);

//! launch diffusion-type amitex simulations
//! \param voxelation result of a voxelation procedure
//! \return the homogenized matrix of coefficients
template <unsigned short DIM>
Matrix<DIM> computeThermalCoeff(const vox::Voxellation<DIM>& voxelation, int nbProcs = 1) {
  Matrix<3> hmat = computeThermalCoeff(voxelation.getGrid(), voxelation.getCoefficents(), nbProcs);
  Matrix<DIM> matrix;
  for (size_t i = 0; i < DIM; i++) {
    for (size_t j = 0; j < DIM; j++) {
      matrix[i][j] = hmat[i][j];
    }
  }
  return matrix;
}

}  // namespace amx
}  // namespace merope

#endif  // _AMITEX_COMPUTE