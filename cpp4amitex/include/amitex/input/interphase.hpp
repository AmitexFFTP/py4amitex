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
  const char* xmlTag() const { return "Interphase"; }
  bool xmlHasBody() const { return true; };
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const;
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
    const char* xmlTag() const { return "Interphase_material"; }
    bool xmlHasBody() const { return false; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const {}
    size_t numM, nZones;
  };
  class InterphaseZoneList {
   public:
    const char* xmlTag() const { return "Interphase_zone_list"; }
    bool xmlHasBody() const { return true; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const;
    size_t numM, nZones;
    List<size_t> zoneList{"ZoneList"};
  };
  std::vector<InterphaseMaterial> materials;
  std::vector<InterphaseZoneList> zoneLists;
};

}  // namespace amitex

#endif  // _AMITEX_INTERPHASE_HEADER
