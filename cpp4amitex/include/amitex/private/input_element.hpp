#ifndef _AMITEX_PRIVATE_INPUT_TEMPLATES_
#define _AMITEX_PRIVATE_INPUT_TEMPLATES_

#if __cpp_concepts >= 201907L
#include <concepts>
#include <ostream>

namespace amitex {

//! \file input_element.h

//! Define the basic requirement for input data that generates XML
template <typename T>
concept InputElement = requires(const T& el, std::ostream& stream) {
  //! \return the tag of the element
  { el.xmlTag() } -> std::convertible_to<const char*>;
  //! \return true if the element has inner text and / or elements
  { el.xmlHasBody() } -> std::same_as<bool>;
  //! Helper function to write the attributes of the XML element
  el.xmlWriteAttributes(stream);
  //! Helper function to write the inner data of the XML element
  el.xmlWriteInner(stream);
};

}  // namespace amitex
#endif

#endif  // _AMITEX_PRIVATE_INPUT_TEMPLATES_