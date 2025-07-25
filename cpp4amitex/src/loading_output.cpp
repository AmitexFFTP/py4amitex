#include "amitex/input/loading_output.hpp"

namespace amitex {

void LoadingOutput::xmlWriteInner(std::ostream& stream) const {
  writeXML(stream, *output);
  if (initLoadExt) writeXML(stream, *initLoadExt);
  for (auto loading : loadings) {
    writeXML(stream, *loading);
  }
}

}  // namespace amitex
