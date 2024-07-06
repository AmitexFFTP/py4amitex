#include "amitex/input/interphase.hpp"

namespace amitex {

void Interphase::xmlWriteInner(std::ostream& stream) const {
  for (const auto& interMat : materials) {
    writeXML(stream, interMat);
  }
  for (const auto& zoneList : zoneLists) {
    writeXML(stream, zoneList);
  }
}

void Interphase::addMaterial(size_t index, size_t nZones) {
  InterphaseMaterial inter;
  inter.numM = index;
  inter.nZones = nZones;
  materials.push_back(inter);
}

void Interphase::addZones(size_t materialId, const std::vector<size_t>& zones) {
  InterphaseZoneList zoneList;
  for (auto zone : zones) zoneList.zoneList.add(zone);
  zoneList.numM = materialId;
  zoneList.nZones = zones.size();
  zoneLists.push_back(std::move(zoneList));
}

void Interphase::InterphaseMaterial::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "numM", numM + 1);
  writeXMLAttributes(stream, "Nzones", nZones);
}

void Interphase::InterphaseZoneList::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "numM", numM + 1);
  writeXMLAttributes(stream, "Nzones", nZones);
}

void Interphase::InterphaseZoneList::xmlWriteInner(std::ostream& stream) const {
  writeXML(stream, zoneList);
}

}  // namespace amitex
