/*
SPDX-License-Identifier: LGPL-3.0-only

(C) copyright 2026 of CEA

author: Guillaume Bareigts <guillaume.bareigts@cea.fr>, CEA
*/
#ifndef _AMITEX_TYPE_ELEMENT_HEADER_
#define _AMITEX_TYPE_ELEMENT_HEADER_

#include <string>
#include <string_view>

namespace amitex {

//! Helper class to defined elements with <ELEMENT Type="…"> signature
class Type {
 public:
  explicit Type(std::string_view tag) : tag_{tag} {}
  explicit Type(std::string_view tag, std::string_view type) : type{type}, tag_{tag} {}
  // explicit Type(const char* tag, const char* type) : type{type}, tag_{tag} {}
  Type& operator=(std::string_view type) {
    this->type = type;
    return *this;
  }
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement(tag_);
    ret.setAttribute("Type", type);
    return ret;
  }

  std::string type = "Default";

 private:
  std::string tag_;
};

}  // namespace amitex

#endif  // _AMITEX_TYPE_ELEMENT_HEADER_
