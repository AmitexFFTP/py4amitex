#ifndef _AMITEX_PRIVATE_INPUT_TEMPLATES_
#define _AMITEX_PRIVATE_INPUT_TEMPLATES_

#if __cpp_concepts >= 201907L
#include <concepts>
#include <ostream>

namespace amitex {

//! \file input_element.h

//! Define the basic requirement for input data that generates XML
template <typename T, TDoc>
concept InputElement = requires(const T& el, TDoc& doc) {
  //! \return XML representation
  { el.toXMLElement(doc) } -> std::same_as<TDoc::Node>;
};

}  // namespace amitex
#endif

#endif  // _AMITEX_PRIVATE_INPUT_TEMPLATES_