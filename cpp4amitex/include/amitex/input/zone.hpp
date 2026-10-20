#ifndef _AMITEX_ZONE_HEADER_
#define _AMITEX_ZONE_HEADER_

#include <vector>

#include "amitex/input/common.hpp"

namespace amitex {

//! Zone, that is a list of voxel positions
class Zone {
 public:
  //! \param gridDims grid dimensions
  static Ptr<Zone> create(GridSize gridDims) { return makePtr<Zone>(Zone{gridDims}); }
  //! \param gridDims grid dimensions
  //! \param position in grid coordinates
  static Ptr<Zone> create(GridSize gridDims, const std::vector<GridPoint>& positions) {
    return makePtr<Zone>(Zone{gridDims, positions});
  }
  //! \param gridDims grid dimensions
  //! \param begin iterator on GridPoint
  //! \param end iterator on GridPoint
  template <typename Iterator>
  static Ptr<Zone> create(GridSize gridDims, Iterator begin, Iterator end) {
    Zone zone{gridDims};
    for (Iterator it = begin; it != end; ++it) zone.add(*it);
    return makePtr<Zone>(std::move(zone));
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
  Zone(GridSize gridDims);
  Zone(GridSize gridDims, const std::vector<GridPoint>& positions);

  std::vector<size_t> linearPositions_;
  GridSize dims;
};

}  // namespace amitex

#endif  // _AMITEX_ZONE_HEADER_
