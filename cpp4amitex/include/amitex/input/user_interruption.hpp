#ifndef _AMITEX_USER_INTERRUPTION_HEADER
#define _AMITEX_USER_INTERRUPTION_HEADER

#include <ostream>

#include "amitex/input/common.hpp"

//! \file user_interruption
//! Used to parametrize a user-provided "user_functions"

namespace amitex {

//! Define user interruptions
class UserInterruption {
 public:
  UserInterruption() : UserInterruption{0.} {}
  UserInterruption(double value) : value_{value}, index_{0} {};
  //! Set index
  void setIndex(size_t index) { index_ = index; }
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("User_interruption");
    ret.setAttribute("Index", index_);
    ret.setAttribute("Value", value_);
    return ret;
  }

 private:
  double value_;
  // Internal for a list of UserInterruption
  size_t index_;
};

}  // namespace amitex

#endif  // _AMITEX_USER_INTERRUPTION_HEADER
