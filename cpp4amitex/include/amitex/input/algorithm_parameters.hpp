#ifndef _AMITEX_ALGORITHM_PARAMETERS_HEADER_
#define _AMITEX_ALGORITHM_PARAMETERS_HEADER_

#include <optional>

#include "amitex/input/algorithm.hpp"
#include "amitex/input/algorithm_composite.hpp"

//! \file algorithm_parameters.hpp

namespace amitex {

//! Group of all algorithm parameters
class AlgorithmParameters {
 public:
  AlgorithmParameters() = default;
  AlgorithmParameters(const Algorithm& algorithm,
                      const std::optional<Mechanics>& mechanics = std::nullopt,
                      const std::optional<Diffusion>& diffusion = std::nullopt)
      : algorithm{algorithm}, mechanics{mechanics}, diffusion{diffusion} {}
  Algorithm algorithm;
  std::optional<Diffusion> diffusion;
  std::optional<Mechanics> mechanics;
  std::optional<AlgorithmLaminate> algorithmLaminate;
  const char* xmlTag() const { return "Algorithm_Parameters"; }
  bool xmlHasBody() const { return true; }
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const;
};

}  // namespace amitex

#endif  // _AMITEX_ALGORITHM_PARAMETERS_HEADER_