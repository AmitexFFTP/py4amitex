#include "amitex/input/zone.hpp"

namespace amitex {

Zone::Zone(GridSize gridDims) : Zone{gridDims, {}} {}

Zone::Zone(GridSize gridDims, const std::vector<GridPoint>& positions)
    : dims{gridDims}, linearPositions_{} {
  for (auto pos : positions) {
    add(pos);
  }
}

void Zone::add(GridPoint pos) {
  linearPositions_.push_back((pos[0] + dims[0] * (pos[1] + dims[1] * pos[2])));
}

void Zone::add(GridLinPoint position) { linearPositions_.push_back(position); }

}  // namespace amitex
