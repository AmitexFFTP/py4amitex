#ifndef _AMITEX_INIT_LOAD_EXT_HEADER_
#define _AMITEX_INIT_LOAD_EXT_HEADER_

#include <vector>

#include "amitex/private/input_element.hpp"
#include "amitex/private/value_element.hpp"

//! \file init_load_ext.hpp
//! Define temperature and external temperature to be imposed during the loading

namespace amitex {

//! Initialization of the temperature and the external parameters
//! \warning
//! If defined, the evolution of the temperature and all the parameters must be specified
//! in the loadings.
class InitLoadExt {
 public:
  InitLoadExt() = default;
  const char* xmlTag() const { return "InitLoadExt"; }
  bool xmlHasBody() const { return true; }
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const;

  //! Set parameter
  //! \param index Index of parameter
  void setParam(size_t index, double value);

  // Temperature
  Value<double> temperature{"T"};

 private:
  class Param {
   public:
    Param() = default;
    bool xmlHasBody() const { return 0; }
    const char* xmlTag() const { return "Param"; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const {};
    size_t index;
    double value = 0;
  };
  std::vector<Param> params;
};

}  // namespace amitex

#endif  // _AMITEX_INIT_LOAD_EXT_HEADER_
