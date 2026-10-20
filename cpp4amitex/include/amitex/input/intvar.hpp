#ifndef _AMITEX_INTVAR__HEADER_
#define _AMITEX_INTVAR__HEADER_

#include "amitex/input/field.hpp"

#include "amitex/private/basic_coeff.hpp"

namespace amitex {

//! Internal variable
class IntVar : public BasicCoeff {
 public:
  IntVar() : BasicCoeff{}, field{} {}
  IntVar(double value) : BasicCoeff{value}, field{} {}
  IntVar(const std::vector<double>& zoneValues) : BasicCoeff{zoneValues}, field{} {}
  IntVar(Field<double>&& field) : field{std::move(field)} {}
  IntVar(const Field<double>& field) : field{field} {}

  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("IntVar");
    ret.setAttribute("Index", id + 1);
    if (!field.empty()) {
      ret.setAttribute("Type", "Variable");
      ret.setAttribute("File", file);
      ret.setAttribute("Format", "vtk");
    } else if (values.size() <= 1) {
      double value = values.size() == 0 ? constantValue() : values[0];
      ret.setAttribute("Type", "Constant");
      ret.setAttribute("Value", value);
    } else {
      ret.setAttribute("Type", "Constant_Zone");
      ret.setAttribute("File", file);
      ret.setAttribute("Format", "Binary");
    }
    if (!name_.empty()) ret.setAttribute("Name", name_);
    return ret;
  }

  //! values as a function of grid coordinates
  Field<double> field;
};

}  // namespace amitex

#endif  // _AMITEX_INTVAR__HEADER_