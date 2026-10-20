#ifndef _AMITEX_ALGORITHM_PARAMETERS_HEADER_
#define _AMITEX_ALGORITHM_PARAMETERS_HEADER_

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

  template <typename TDoc>
  TDoc toXMLDocument() const {
    TDoc doc{"Algorithm_Parameters"};
    auto root = doc.getRoot();
    root.appendChild(algorithm->toXMLElement(doc));
    if (diffusion) root.appendChild(diffusion->toXMLElement(doc));
    if (mechanics) root.appendChild(mechanics->toXMLElement(doc));
    if (algorithmLaminate) root.appendChild(algorithmLaminate->toXMLElement(doc));
    return doc;
  }

 private:
  AlgorithmParameters(Ptr<Algorithm> algorithm, Ptr<Mechanics> mechanics = nullptr,
                      Ptr<Diffusion> diffusion = nullptr)
      : algorithm{algorithm}, mechanics{mechanics}, diffusion{diffusion} {}
};

}  // namespace amitex

#endif  // _AMITEX_ALGORITHM_PARAMETERS_HEADER_