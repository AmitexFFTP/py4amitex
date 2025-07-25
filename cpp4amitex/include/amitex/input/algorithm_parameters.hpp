#ifndef _AMITEX_ALGORITHM_PARAMETERS_HEADER_
#define _AMITEX_ALGORITHM_PARAMETERS_HEADER_

#include <optional>

#include "amitex/input/algorithm.hpp"
#include "amitex/input/algorithm_composite.hpp"
#include "amitex/input/common.hpp"

//! \file algorithm_parameters.hpp

namespace amitex {

//! Group of all algorithm parameters
class AlgorithmParameters {
 public:
  static Ptr<AlgorithmParameters> create(Ptr<Algorithm> algorithm,
                                         Ptr<Mechanics> mechanics = nullptr,
                                         Ptr<Diffusion> diffusion = nullptr) {
    return makePtr<AlgorithmParameters>(AlgorithmParameters{algorithm, mechanics, diffusion});
  }
  Ptr<Algorithm> algorithm;
  Ptr<Diffusion> diffusion;
  Ptr<Mechanics> mechanics;
  Ptr<AlgorithmLaminate> algorithmLaminate;
  const char* xmlTag() const { return "Algorithm_Parameters"; }
  bool xmlHasBody() const { return true; }
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const;

 private:
  AlgorithmParameters(Ptr<Algorithm> algorithm, Ptr<Mechanics> mechanics = nullptr,
                      Ptr<Diffusion> diffusion = nullptr)
      : algorithm{algorithm},
        mechanics{mechanics},
        diffusion{diffusion},
        algorithmLaminate{nullptr} {}
};

}  // namespace amitex

#endif  // _AMITEX_ALGORITHM_PARAMETERS_HEADER_