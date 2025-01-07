#include "amitex/input/material_composite.hpp"

#include <cmath>

#include "amitex/errors.hpp"
#include "amitex/private/xml_utils.hpp"

namespace amitex {

Composite::Composite(const std::vector<size_t>& phaseIndices, const std::string& law)
    : phaseIndices_{phaseIndices}, law_{law}, pos_{}, volfracs_{}, S_{}, N_{}, T_{} {
  size_t nphases = phaseIndices.size();
  volfracs_.resize(nphases);
  for (size_t i = 0; i < 3; i++) N_[i].resize((nphases * (nphases - 1)) / 2);
  for (size_t i = 0; i < 3; i++) T_[i].resize((nphases * (nphases - 1)) / 2);
  S_.resize((nphases * (nphases - 1)) / 2);
}

void Composite::addVoxel(GridLinPoint position, const std::vector<double>& phi) {
  pos_.push_back(position);
  if (phi.size() != volfracs_.size()) throw InputError{"wrong number of phases in addVoxel"};
  for (size_t i = 0; i < phi.size(); i++) {
    volfracs_[i].push_back(phi[i]);
  }
}

void Composite::addVoxel(GridLinPoint position, const std::vector<double>& phi,
                         const std::vector<InterfaceGeometry>& geom) {
  pos_.push_back(position);
  if (phi.size() != volfracs_.size())
    throw InputError{"wrong number of phases in " + std::string{__func__}};
  for (size_t i = 0; i < phi.size(); i++) {
    volfracs_[i].push_back(phi[i]);
  }
  size_t idx = 0;
  for (size_t i = 0; i < phi.size(); i++) {
    for (size_t j = i + 1; j < phi.size(); j++) {
      for (size_t k = 0; k < 3; k++) N_[k][idx].push_back(geom[idx].normal[k]);
      for (size_t k = 0; k < 3; k++) T_[k][idx].push_back(geom[idx].tangent[k]);
      S_[idx].push_back(geom[idx].surface);
      idx++;
    }
  }
}

void MaterialComposite::xmlWriteInner(std::ostream& stream) const {
  writeXML(stream, coeffComposite);
}

void MaterialComposite::CoeffComposite::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "directory", directory);
}

}  // namespace amitex
