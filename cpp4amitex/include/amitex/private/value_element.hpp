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
  explicit Value(const std::string& tag) : Value{tag, std::nullopt} {}
  Value(const std::string& tag, const T& value) : value{value}, tag_{tag} {}
  Value(const std::string& tag, const std::optional<T>& value) : value{value}, tag_{tag} {}
  operator T() const { return value.value(); }
  Value& operator=(const T& value) {
    this->value = value;
    return *this;
  }
  const char* xmlTag() const { return tag_.c_str(); }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const {
    if (value)
      writeXMLAttributes(stream, "Value", value.value());
    else
      writeXMLAttributes(stream, "Value", "Default");
  }
  void xmlWriteInner(std::ostream& stream) const {}
  //! Get the value
  std::optional<T> value;

 private:
  std::string tag_;
};

}  // namespace amitex

#endif  // _AMITEX_VALUE_ELEMENT_HEADER_
