#ifndef _AMITEX_ZONE_HEADER_
#define _AMITEX_ZONE_HEADER_

#include <vector>

#include "amitex/input/common.hpp"

namespace amitex {

class Zone {
 public:
  //! \param position list of grid coordinates
  //! \param gridDims grid dimensions
  Zone(GridSize gridDims);
  //! \param gridDims grid dimensions
  //! \param position in grid coordinates
  Zone(GridSize gridDims, const std::vector<GridPoint>& positions);
  //! \param gridDims grid dimensions
  //! \param begin iterator on GridPoint
  //! \param end iterator on GridPoint
  template <typename Iterator>
  Zone(GridSize gridDims, Iterator begin, Iterator end) : Zone{gridDims} {
    for (Iterator it = begin; it != end; ++it) add(*it);
  }

  //! add a voxel to a zone
  //! \param position voxel grid coordinate
  void add(GridPoint position);
  //! add a voxel to a zone
  //! \param position voxel grid linearized position
  void add(GridLinPoint position);

  //! \return number of voxels
  size_t numberVoxels() const { return linearPositions_.size(); }

  //! linear index of positions
  const std::vector<size_t>& linearPositions() const { return linearPositions_; }

 private:
  std::vector<size_t> linearPositions_;
  GridSize dims;
};

}  // namespace amitex

#endif  // _AMITEX_ZONE_HEADER_
