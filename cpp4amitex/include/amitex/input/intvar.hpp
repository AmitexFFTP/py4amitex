#ifndef _AMITEX_INTVAR__HEADER_
#define _AMITEX_INTVAR__HEADER_

#include "amitex/input/field.hpp"

#include "amitex/private/basic_coeff.hpp"

namespace amitex {

//! Internal variable
class IntVar : public BasicCoeff {
 public:
  IntVar() : BasicCoeff{} {}
  IntVar(double value) : BasicCoeff{value} {}
  IntVar(const std::vector<double>& zoneValues) : BasicCoeff{zoneValues} {}
  IntVar(Field<double>&& field) : field{field} {}
  IntVar(const Field<double>& field) : field{field} {}
  const char* xmlTag() const { return "IntVar"; }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const {}

  //! values as a function of grid coordinates
  Field<double> field;
};

}  // namespace amitex

#endif  // _AMITEX_INTVAR__HEADER_