#ifndef _AMITEX_INTERPHASE_HEADER
#define _AMITEX_INTERPHASE_HEADER

#include "amitex/private/input_element.hpp"
#include "amitex/private/list_element.hpp"

namespace amitex {

//! Definition of an "interphase" material.
//! \note Used for input file generation, from material and composite definitions,
//! generally of not to be use directly.
class Interphase {
 public:
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Interphase");
    for (const auto& interMat : materials) {
      ret.appendChild(interMat.toXMLElement(doc));
    }
    for (const auto& zoneList : zoneLists) {
      ret.appendChild(zoneList.toXMLElement(doc));
    }
    return ret;
  }

  //! add a material to the list of interphases
  //! \param index index of the material
  //! \param nZones number of zones
  void addMaterial(size_t index, size_t nZones);
  //! add a zone of a material to the list of interphases
  //! \param materialId index of the material
  //! \param zones indices of zones
  void addZones(size_t materialId, const std::vector<size_t>& zones);

  size_t numberMaterials() const { return materials.size(); }
  size_t numberZoneLists() const { return zoneLists.size(); }

 private:
  class InterphaseMaterial {
   public:
    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement("Interphase_material");
      ret.setAttribute("numM", numM + 1);
      ret.setAttribute("Nzones", nZones);
      return ret;
    }

    size_t numM, nZones;
  };
  class InterphaseZoneList {
   public:
    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement("Interphase_zone_list");
      ret.setAttribute("numM", numM + 1);
      ret.setAttribute("Nzones", nZones);
      ret.appendChild(zoneList.toXMLElement(doc));
      return ret;
    }
    size_t numM, nZones;
    List<size_t> zoneList{"ZoneList"};
  };
  std::vector<InterphaseMaterial> materials;
  std::vector<InterphaseZoneList> zoneLists;
};

}  // namespace amitex

#endif  // _AMITEX_INTERPHASE_HEADER
