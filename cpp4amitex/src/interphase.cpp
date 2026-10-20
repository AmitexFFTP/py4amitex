#include "amitex/input/interphase.hpp"

namespace amitex {

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

}  // namespace amitex
