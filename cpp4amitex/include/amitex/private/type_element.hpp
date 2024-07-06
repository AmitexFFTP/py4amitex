#ifndef _AMITEX_TYPE_ELEMENT_HEADER_
#define _AMITEX_TYPE_ELEMENT_HEADER_

#include <iomanip>
#include <optional>
#include <string_view>
#include <vector>

#include "amitex/private/input_element.hpp"
#include "amitex/private/xml_utils.hpp"

namespace amitex {

//! Helper class to defined elements with <ELEMENT Type="…"> signature
class Type {
 public:
  explicit Type(const char* tag) : tag_{tag} {}
  explicit Type(const char* tag, std::string_view type) : type{type}, tag_{tag} {}
  // explicit Type(const char* tag, const char* type) : type{type}, tag_{tag} {}
  Type& operator=(std::string_view type) {
    this->type = type;
    return *this;
  }
  const char* xmlTag() const { return tag_; }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const { writeXMLAttributes(stream, "Type", type); }
  void xmlWriteInner(std::ostream& stream) const {}

  std::string type = "Default";

 private:
  const char* tag_;
};

}  // namespace amitex

#endif  // _AMITEX_TYPE_ELEMENT_HEADER_
