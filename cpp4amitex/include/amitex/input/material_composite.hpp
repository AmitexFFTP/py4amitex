#ifndef _AMITEX_MATERIAL_COMPOSITE_HEADER_
#define _AMITEX_MATERIAL_COMPOSITE_HEADER_

#include <ostream>
#include <vector>

#include "amitex/input/zone.hpp"

#include "amitex/private/input_element.hpp"

//! \file material_composite.hpp
//! Definition of composite materials

namespace amitex {

//! Define an interface in a composite voxel
struct InterfaceGeometry {
  Vector3D normal;
  Vector3D tangent;
  double surface;
};

//! Composite material
//! \note the coefficients are set in the pure material (for ex \ref Material::setCoeffComposite)
class Composite {
 public:
  static constexpr double maxVolumeFraction = 1.0e-4;  // AMITEX requirement

  //! \param materialIndices indices of pure materials (as defined for Materials) of each phase
  //! \param law averaging law
  Composite(const std::vector<size_t>& materialIndices, const std::string& law = "");

  //! add a voxel to the material
  //! \param position linearized position
  //! \param phi volume fractions of each phase
  void addVoxel(GridLinPoint position, const std::vector<double>& phi,
                const std::vector<size_t>& zones = {});

  //! add a voxel to the material
  //! \param position linearized position
  //! \param phi volume fractions of each phase
  //! \param geom interface geometry for each couple of phases (order i<j: 00 01 …)
  void addVoxel(GridLinPoint position, const std::vector<double>& phi,
                const std::vector<InterfaceGeometry>& geom, const std::vector<size_t>& zones = {});

  //! Get composite law
  const std::string& law() { return law_; }
  //! Set averaging law
  void setLaw(const std::string& law) { law_ = law; }

  //! Get number of phases
  size_t numberPhases() const { return phaseIndices_.size(); }
  //! Get the material indices for each phase
  const std::vector<size_t>& materialIndices() const { return phaseIndices_; }
  //! Get the linearized positions of voxels constituting the material
  const std::vector<GridLinPoint>& positions() const { return pos_; }
  std::vector<GridLinPoint>& positions() { return pos_; }
  //! Get the volume fractions of a phase
  //! \param index phase index
  const std::vector<double>& volumeFractions(size_t index) const { return volfracs_.at(index); }
  std::vector<double>& volumeFractions(size_t index) { return volfracs_.at(index); }
  //! Get the zones index of a phase
  //! \param index phase index
  const std::vector<size_t>& zone(size_t index) const { return zones_.at(index); }
  std::vector<size_t>& zone(size_t index) { return zones_.at(index); }

  //! Get normals
  const std::array<std::vector<std::vector<double>>, 3>& normals() const { return N_; }
  //! Get tangents
  const std::array<std::vector<std::vector<double>>, 3>& tangents() const { return T_; }
  //! Get surfaces
  const std::vector<std::vector<double>>& surfaces() const { return S_; };

 private:
  std::vector<size_t> phaseIndices_;
  std::vector<GridLinPoint> pos_;
  std::vector<std::vector<double>> volfracs_;
  std::vector<std::vector<size_t>> zones_;
  // Normals, tangents and surfaces defined in order 12 13 …
  std::array<std::vector<std::vector<double>>, 3> N_;
  std::array<std::vector<std::vector<double>>, 3> T_;
  std::vector<std::vector<double>> S_;
  std::string law_;
};

//! Define all composite materials
class MaterialComposite {
 public:
  MaterialComposite() = default;
  const char* xmlTag() const { return "Material_composite"; }
  bool xmlHasBody() const { return true; }
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const;

  //! Set directory where generated files wil be located
  void setDirectory(std::string_view path) { coeffComposite.directory = path; };

  //! Get number of defined composite materials
  size_t numberMaterials() const { return materials_.size(); }
  //! Add a composite material
  void add(Composite&& composite) { materials_.push_back(std::move(composite)); }
  void add(const Composite& composite) { materials_.push_back(composite); }

  //! Get a composite material
  //! \param index of the composite material
  const Composite& at(size_t index) const { return materials_.at(index); }
  Composite& at(size_t index) { return materials_.at(index); }

 private:
  class CoeffComposite {
   public:
    const char* xmlTag() const { return "Coeff_composite"; }
    bool xmlHasBody() const { return false; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const {}
    std::string directory;
  };
  CoeffComposite coeffComposite;
  std::vector<Composite> materials_;
};

}  // namespace amitex

#endif  // _AMITEX_MATERIAL_COMPOSITE_HEADER_