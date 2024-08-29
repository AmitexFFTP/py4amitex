#ifndef _AMITEX_ALGORITHM_COMPOSITE_HEADER_
#define _AMITEX_ALGORITHM_COMPOSITE_HEADER_

#include "amitex/input/algorithm.hpp"
#include "amitex/private/input_element.hpp"
#include "amitex/private/value_element.hpp"

//! \file algorithm_composite.hpp

namespace amitex {

//! Special algorithm parameters for simulations with composite law (laminate, reuss, …)
class AlgorithmLaminate {
 public:
  AlgorithmLaminate() = default;
  const char* xmlTag() const { return "Algorithm_laminate"; }
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Algorithm_laminate");
    ret.appendChild(convergenceCriterion.toXMLElement(doc));
    ret.appendChild(convergenceAcceleration.toXMLElement(doc));
    if (initializationType.value) ret.appendChild(initializationType.toXMLElement(doc));
    if (nIncrements.value) ret.appendChild(nIncrements.toXMLElement(doc));
    return ret;
  }

  //! Convergence criterion (>1e-4 and  >1e-1)
  Value<double> convergenceCriterion{"Convergence_Criterion"};
  //! Toggle convergence acceleration
  ConvergenceAcceleration convergenceAcceleration;
  //! Initialization type (Proportionnal, Linear, Default (=Linear)
  Value<std::string> initializationType{"Initialisation_type"};
  //! Number of substeps for laminate law
  Value<int> nIncrements{"N_increments"};
};

}  // namespace amitex

#endif  // _AMITEX_ALGORITHM_COMPOSITE_HEADER_
