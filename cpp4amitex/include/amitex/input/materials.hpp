#ifndef _AMITEX_MATERIALS_HEADER_
#define _AMITEX_MATERIALS_HEADER_

//! \file materials.hpp
//! Set of all materials (regular, composite, reference…)

#include "amitex/input/interphase.hpp"
#include "amitex/input/material.hpp"
#include "amitex/input/material_composite.hpp"

namespace amitex {

//! Reference material for mechanics
class ReferenceMaterial {
 public:
  static Ptr<ReferenceMaterial> create(double lambda0, double mu0) {
    return makePtr<ReferenceMaterial>(ReferenceMaterial{lambda0, mu0});
  }
  const char* xmlTag() const { return "Reference_Material"; }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const {}

 private:
  ReferenceMaterial(double lambda0, double mu0) : lambda0{lambda0}, mu0{mu0} {}
  double lambda0, mu0;
};

//! Reference material for diffusion
class ReferenceMaterialD {
 public:
  static Ptr<ReferenceMaterialD> create(double K0) {
    return makePtr<ReferenceMaterialD>(ReferenceMaterialD{K0});
  }
  const char* xmlTag() const { return "Reference_MaterialD"; }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const {}

 private:
  ReferenceMaterialD(double K0) : K0{K0} {}
  double K0;
};

//! Definition of all materials
class Materials {
 public:
  static Ptr<Materials> create() { return makePtr<Materials>(Materials{}); }
  const char* xmlTag() const { return "Materials"; }
  bool xmlHasBody() const;
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const;
  //! Get the number of materials
  size_t numberMaterials() const { return materials.size(); }
  //! Set the number of materials (can substract materials or add empty materials )
  //! \param num desired number of materials
  void setNumberMaterials(size_t num);
  //! Add a material
  void add(Ptr<Material> material) {
    materials.push_back(material);
    setLastId();
  }
  //! Get a material
  //! \param id material index
  Ptr<Material> material(size_t id) const { return materials.at(id); }
  //! Get a material
  //! \param id material index
  Ptr<Material> at(size_t id) const { return materials.at(id); }

  //! Get the number composite of materials
  size_t numberComposites() const { return composites.numberMaterials(); }
  //! Add a composite material
  void add(Ptr<Composite> composite) {
    composites.add(composite);
    setLastId();
  }
  Ptr<Composite> composite(int id) const { return composites.at(id); }

  auto begin() { return materials.begin(); }
  auto end() { return materials.end(); }
  const auto begin() const { return materials.begin(); }
  const auto end() const { return materials.end(); }

  //! reference material (mechanics)
  Ptr<ReferenceMaterial> referenceMaterial = nullptr;
  //! reference material (diffusion)
  Ptr<ReferenceMaterialD> referenceMaterialD = nullptr;
  //! Composite materials
  MaterialComposite composites;
  //! Interphase materials
  //! \note In general this element is constructed by \ref Input::generateFiles
  std::optional<Interphase> interphase;

 private:
  Materials()
      : materials{},
        referenceMaterial{nullptr},
        referenceMaterialD{nullptr},
        composites{},
        interphase{std::nullopt} {};
  void setLastId() { materials[materials.size() - 1]->setIndex(materials.size() - 1); }
  std::vector<Ptr<Material>> materials;
};

}  // namespace amitex

#endif  // _AMITEX_MATERIALS_HEADER_