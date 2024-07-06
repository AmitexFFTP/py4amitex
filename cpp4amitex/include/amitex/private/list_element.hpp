#ifndef _AMITEX_LIST_ELEMENT_HEADER_
#define _AMITEX_LIST_ELEMENT_HEADER_

#include <iomanip>
#include <optional>
#include <vector>

#include "amitex/private/input_element.hpp"
#include "amitex/private/xml_utils.hpp"

namespace amitex {

//! Helper class to define elements containing space-separated list of values
//! (like <ELEMENT>value0 value1 … </ELEMENT>)
template <typename T>
class List {
 public:
  List(const char* tag) : tag_{tag} {}
  const char* xmlTag() const { return tag_; }
  bool xmlHasBody() const { return values.size() > 0; }
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const {
    for (const auto& value : values) {
      // Lack of precision affected comparison with original XMLs
      stream << std::setprecision(12) << value << ' ';
    }
  }
  void add(const T& value) { values.push_back(value); }
  void add(T&& value) { values.push_back(value); }
  //! Get the number of listed values
  size_t numberValues() const { return values.size(); }

 protected:
  std::vector<T> values;

 private:
  const char* tag_;
};

}  // namespace amitex

#endif  // _AMITEX_LIST_ELEMENT_HEADER_
