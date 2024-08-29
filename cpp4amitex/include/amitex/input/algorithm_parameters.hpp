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

  template <typename TDoc>
  TDoc toXMLDocument() const {
    TDoc doc{"Algorithm_Parameters"};
    auto root = doc.getRoot();
    root.appendChild(algorithm.toXMLElement(doc));
    if (diffusion) root.appendChild(diffusion.value().toXMLElement(doc));
    if (mechanics) root.appendChild(mechanics.value().toXMLElement(doc));
    if (algorithmLaminate) root.appendChild(algorithmLaminate.value().toXMLElement(doc));
    return doc;
  }
};

}  // namespace amitex

#endif  // _AMITEX_ALGORITHM_PARAMETERS_HEADER_