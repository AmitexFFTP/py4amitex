#include "amitex/input/algorithm_parameters.hpp"

#include "amitex/private/xml_utils.hpp"

namespace amitex {

void AlgorithmParameters::xmlWriteInner(std::ostream& stream) const {
  writeXML(stream, *algorithm);
  if (diffusion) writeXML(stream, *diffusion);
  if (mechanics) writeXML(stream, *mechanics);
  if (algorithmLaminate) writeXML(stream, *algorithmLaminate);
}

}  // namespace amitex
