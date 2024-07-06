#include <amitex/input/output.hpp>

namespace amitex {

void VtkStressStrain::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Strain", strain);
  writeXMLAttributes(stream, "Stress", stress);
}

void VtkFluxDGradD::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "FluxD", fluxd);
  writeXMLAttributes(stream, "GradD", gradd);
}

void Output::xmlWriteInner(std::ostream& stream) const {
  if (vtkStressStrain) writeXML(stream, vtkStressStrain.value());
  if (vtkFluxDGradD) writeXML(stream, vtkFluxDGradD.value());
  for (const auto& zone : zones) writeXML(stream, zone);
  for (const auto& ivarList : intVarList) writeXML(stream, ivarList);
}

bool Output::Zone::xmlHasBody() const { return varIntList.numberValues() > 0; };

void Output::Zone::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "numM", numM + 1);
}

void Output::Zone::xmlWriteInner(std::ostream& stream) const {
  if (varIntList.numberValues() > 0) writeXML(stream, varIntList);
}

void Output::Zone::setVarIntList(const std::vector<size_t>& list) {
  for (auto var : list) varIntList.add(var + 1);
}

void Output::VtkIntVarList::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "numM", numM + 1);
}

}  // namespace amitex
