/*
SPDX-License-Identifier: LGPL-3.0-only

(C) copyright 2026 of CEA

author: Guillaume Bareigts <guillaume.bareigts@cea.fr>, CEA
*/
#ifndef _AMITEX_LIST_ELEMENT_HEADER_
#define _AMITEX_LIST_ELEMENT_HEADER_

#include <iomanip>
#include <sstream>
#include <string_view>
#include <vector>

namespace amitex {

//! Helper class to define elements containing space-separated list of values
//! (like <ELEMENT>value0 value1 … </ELEMENT>)
template <typename T>
class List {
 public:
  List(std::string_view tag) : tag_{tag} {}
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement(tag_);
    std::stringstream stream;
    for (const auto& value : values) {
      // Lack of precision affected comparison with original XMLs
      stream << std::setprecision(12) << value << ' ';
    }
    ret.appendChild(doc.createText(stream.str()));
    return ret;
  }
  void add(const T& value) { values.push_back(value); }
  void add(T&& value) { values.push_back(std::move(value)); }
  //! Get the number of listed values
  size_t numberValues() const { return values.size(); }

 protected:
  std::vector<T> values;

 private:
  std::string tag_;
};

}  // namespace amitex

#endif  // _AMITEX_LIST_ELEMENT_HEADER_
