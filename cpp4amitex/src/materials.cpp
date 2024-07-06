#include "amitex/input/materials.hpp"

namespace amitex {

void ReferenceMaterial::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Lambda0", lambda0);
  writeXMLAttributes(stream, "Mu0", mu0);
}

void ReferenceMaterialD::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "K0", K0);
}

bool Materials::xmlHasBody() const {
  size_t nb = materials.size();
  if (referenceMaterial) nb += 1;
  if (referenceMaterialD) nb += 1;
  if (composites.numberMaterials() > 0) nb += 1;
  return nb > 0;
}

void Materials::xmlWriteInner(std::ostream& stream) const {
  if (referenceMaterial) writeXML(stream, referenceMaterial.value());
  if (referenceMaterialD) writeXML(stream, referenceMaterialD.value());
  for (const auto& mat : materials) {
    writeXML(stream, mat);
  }
  if (composites.numberMaterials() > 0) writeXML(stream, composites);
  if (interphase) writeXML(stream, interphase.value());
}

}  // namespace amitex
