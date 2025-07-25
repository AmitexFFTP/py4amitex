#ifndef _AMITEX_ALGORITHM_COMPOSITE_HEADER_
#define _AMITEX_ALGORITHM_COMPOSITE_HEADER_

#include "amitex/input/algorithm.hpp"
#include "amitex/input/common.hpp"
#include "amitex/private/input_element.hpp"
#include "amitex/private/value_element.hpp"

//! \file algorithm_composite.hpp

namespace amitex {

//! Special algorithm parameters for simulations with composite law (laminate, reuss, …)
class AlgorithmLaminate {
 public:
  static Ptr<AlgorithmLaminate> create() { return makePtr<AlgorithmLaminate>(AlgorithmLaminate{}); }
  const char* xmlTag() const { return "Algorithm_laminate"; }
  bool xmlHasBody() const { return true; }
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const;

  //! Convergence criterion (>1e-4 and  >1e-1)
  Value<double> convergenceCriterion{"Convergence_Criterion"};
  //! Toggle convergence acceleration
  ConvergenceAcceleration convergenceAcceleration{};
  //! Initialization type (Proportionnal, Linear, Default (=Linear)
  Value<std::string> initializationType{"Initialisation_type"};
  //! Number of substeps for laminate law
  Value<int> nIncrements{"N_increments"};

 private:
  AlgorithmLaminate() = default;
};

}  // namespace amitex

#endif  // _AMITEX_ALGORITHM_COMPOSITE_HEADER_
