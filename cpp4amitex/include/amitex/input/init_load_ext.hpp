#ifndef _AMITEX_INIT_LOAD_EXT_HEADER_
#define _AMITEX_INIT_LOAD_EXT_HEADER_

#include <vector>

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

  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("InitLoadExt");
    if (temperature.value) ret.appendChild(temperature.toXMLElement(doc));
    for (const auto& param : params) ret.appendChild(param.toXMLElement(doc));
    return ret;
  }

  //! Set parameter
  //! \param index Index of parameter
  void setParam(size_t index, double value);

  // Temperature
  Value<double> temperature{"T"};

 private:
  class Param {
   public:
    Param() = default;

    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement("Param");
      ret.setAttribute("Index", index + 1);
      ret.setAttribute("Value", value);
      return ret;
    }

    size_t index;
    double value = 0;
  };
  std::vector<Param> params;
};

}  // namespace amitex

#endif  // _AMITEX_INIT_LOAD_EXT_HEADER_
