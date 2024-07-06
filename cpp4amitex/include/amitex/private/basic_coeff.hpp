#ifndef _AMITEX_BASIC_COEFF_HEADER_
#define _AMITEX_BASIC_COEFF_HEADER_

#include <string_view>
#include <vector>

#include "amitex/private/input_element.hpp"
#include "amitex/private/xml_utils.hpp"

//! \file basic_coeff.h
//! Template for defining 'Coeff'-like elements in the input XML

namespace amitex {

class BasicCoeff {
 public:
  BasicCoeff() {}
  //! Define a coefficient by a constant value
  BasicCoeff(double value) : constantValue_{value} {}

  //! Define a coefficient by a constant value per zone
  //! \param values list of values by increasing zone index
  //! \note It is recommanded to use \ref Material::addZone for this purpose
  BasicCoeff(const std::vector<double>& values) : values{values} {}
  BasicCoeff(std::vector<double>&& values) : values{values} {}

  const char* xmlTag() const { return "BasicCoeff"; }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const {}

  //! \return the value of the coefficient
  double constantValue() const { return constantValue_; }

  //! \return the value per zone of the coefficient
  const std::vector<double>& zoneValues() const { return values; }
  std::vector<double>& zoneValues() { return values; }

  //! set index (corresponding to law). In general you do not need to call it.
  void setIndex(int index) { id = index; }
  //! set path of coefficient file. In general you do not need to call it.
  void setFile(std::string_view path) { this->file = path; }

  //! Attach a name to a coefficient (optional)
  void setName(std::string_view name) { this->name_ = name; }

 protected:
  int id = -1;
  double constantValue_ = 0.0;
  std::vector<double> values;
  std::string file;
  std::string name_;
};

}  // namespace amitex

#endif  // _AMITEX_BASIC_COEFF_HEADER_