#include "amitex/input/algorithm_composite.hpp"

#include "amitex/private/xml_utils.hpp"

namespace amitex {

void AlgorithmLaminate::xmlWriteInner(std::ostream& stream) const {
  writeXML(stream, convergenceCriterion);
  writeXML(stream, convergenceAcceleration);
  if (initializationType.value) writeXML(stream, initializationType);
  if (nIncrements.value) writeXML(stream, nIncrements);
  if (nMaxSubdivision.value) writeXML(stream, nMaxSubdivision);
}

}  // namespace amitex
