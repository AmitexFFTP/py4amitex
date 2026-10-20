#ifndef _AMITEX_VALUE_ELEMENT_HEADER_
#define _AMITEX_VALUE_ELEMENT_HEADER_

#include <optional>
#include <string>
#include <string_view>

namespace amitex {

//! Helper class to easily interact with elements of <ELEMENT Value="…"> signature
template <typename T>
class Value {
 public:
  explicit Value(std::string_view tag) : Value{tag, std::nullopt} {}
  Value(std::string_view tag, const T& value) : value{value}, tag_{tag} {}
  Value(std::string_view tag, const std::optional<T>& value) : value{value}, tag_{tag} {}
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
  std::string tag_;
};

}  // namespace amitex

#endif  // _AMITEX_VALUE_ELEMENT_HEADER_
