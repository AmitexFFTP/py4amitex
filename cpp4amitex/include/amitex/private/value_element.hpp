#ifndef _AMITEX_VALUE_ELEMENT_HEADER_
#define _AMITEX_VALUE_ELEMENT_HEADER_

#include <iomanip>
#include <optional>
#include <vector>

#include "amitex/private/input_element.hpp"
#include "amitex/private/xml_utils.hpp"

namespace amitex {

//! Helper class to easily interact with elements of <ELEMENT Value="…"> signature
template <typename T>
class Value {
 public:
  explicit Value(const char* tag) : tag_{tag} {}
  Value(const char* tag, const T& value) : value{value}, tag_{tag} {}
  Value(const char* tag, const std::optional<T>& value) : value{value}, tag_{tag} {}
  operator T() const { return value.value(); }
  Value& operator=(const T& value) {
    this->value = value;
    return *this;
  }
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement(tag_);
    if (value)
      ret.setAttribute("Value", value.value());
    else {
      ret.setAttribute("Value", "Default");
    }
    return ret;
  }

  //! Get the value
  std::optional<T> value;

 private:
  const char* tag_;
};

}  // namespace amitex

#endif  // _AMITEX_VALUE_ELEMENT_HEADER_
