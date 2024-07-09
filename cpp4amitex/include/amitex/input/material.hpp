#ifndef _AMITEX_MATERIAL_HEADER_
#define _AMITEX_MATERIAL_HEADER_

#include <optional>
#include <string_view>

#include "amitex/input/field.hpp"
#include "amitex/input/intvar.hpp"
#include "amitex/input/zone.hpp"

#include "amitex/private/basic_coeff.hpp"
#include "amitex/private/input_element.hpp"

//! \file materials.hpp
//! Definition of a "pure" material

namespace amitex {

class Coeff : public BasicCoeff {
 public:
  const char* xmlTag() const { return "Coeff"; }
};
class CoeffK : public BasicCoeff {
 public:
  const char* xmlTag() const { return "CoeffK"; }
};
class CoeffComposite : public BasicCoeff {
 public:
  const char* xmlTag() const { return "Coeff_composite"; }
};

//! Definition of a "pure" material
class Material {
 public:
  Material() = default;
  Material(int id) : id{id} {}

  const char* xmlTag() const { return "Material"; }
  bool xmlHasBody() const {
    return (coeffs.size() + coeffKs.size() + coeffComposites.size() + intvars_.size()) > 0;
  }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const;

  //! Set behavior law for mechanics
  //! \param law name of law
  //! \param lib path to the library implementing the law
  void setLaw(std::string_view law, std::string_view lib = "") {
    this->law = law;
    this->lib = lib;
  }
  //! Set behavior law for diffusion
  //! \param law name of law
  //! \param lib path to the library implementing the law
  void setLawK(std::string_view law, std::string_view lib = "") {
    this->lawK = law;
    this->libK = lib;
  }
  //! Set mechanics coefficient constant value
  //! \param id index of coefficient
  //! \param coeff value of coefficient
  void setCoeff(size_t id, double coeff) {
    coeffs.at(id) = Coeff{coeff};
    coeffs.at(id).setIndex(id);
  }
  //! Set mechanics coefficient constant zone values
  //! \param id index of coefficient
  //! \param coeff value of coefficient
  void setCoeffZone(size_t id, const std::vector<double> zoneCoeffs) {
    coeffs.at(id) = std::move(Coeff{zoneCoeffs});
    coeffs.at(id).setIndex(id);
  }
  //! Set mechanics coefficient constant zone values
  //! \param id index of coefficient
  //! \param binPath path to BIN file
  void setCoeffZoneFromBin(size_t id, const std::filesystem::path& binPath);
  //! Set all mechanics coefficients
  //! \param coefficients values of coefficient
  void setCoeffs(const std::vector<double>& coefficents) {
    coeffs.resize(0);
    for (size_t i = 0; i < coefficents.size(); i++) {
      coeffs.push_back(Coeff{coefficents[i]});
      coeffs[i].setIndex(i);
    }
  }
  //! Set name of coefficient (optional)
  //! \param name name
  void setCoeffName(size_t id, const std::string& name) { coeffs.at(id).setName(name); }
  //! Set diffusion coefficient constant value
  //! \param id index of coefficient
  //! \param coeff value of coefficients
  void setCoeffK(size_t id, double coeff) {
    coeffKs.at(id) = CoeffK{coeff};
    coeffKs.at(id).setIndex(id);
  }
  //! Set diffusion coefficient constant zone values
  //! \param id index of coefficient
  //! \param coeff value of coefficient
  void setCoeffKZone(size_t id, const std::vector<double> zoneCoeffs) {
    coeffKs.at(id) = std::move(CoeffK{zoneCoeffs});
    coeffKs.at(id).setIndex(id);
  }
  //! Set diffusion coefficient constant zone values
  //! \param id index of coefficient
  //! \param binPath path to BIN file
  void setCoeffKZoneFromBin(size_t id, const std::filesystem::path& binPath);
  //! Set all diffusion coefficients
  //! \param coefficients values of coefficients
  void setCoeffKs(const std::vector<double>& coefficents) {
    coeffKs.resize(0);
    for (size_t i = 0; i < coefficents.size(); i++) {
      coeffKs.push_back(CoeffK{coefficents[i]});
      coeffKs[i].setIndex(i);
    }
  }
  //! Set name of diffusion coefficient (optional)
  //! \param name name
  void setCoeffKName(size_t id, const std::string& name) { coeffKs.at(id).setName(name); }
  //! Set mechanics coefficient constant value (for composite materials)
  //! \param id index of coefficient
  //! \param coeff value of coefficient
  void setCoeffComposite(int id, double coeff) {
    coeffComposites.at(id) = std::move(CoeffComposite{coeff});
    coeffComposites.at(id).setIndex(id);
  }
  //! Set mechanics coefficient constant zone values (for composite materials)
  //! \param id index of coefficient
  //! \param coeff value of coefficient
  void setCoeffCompositeZone(size_t id, const std::vector<double> zoneCoeffs) {
    coeffComposites.at(id) = CoeffComposite{zoneCoeffs};
    coeffComposites.at(id).setIndex(id);
  }
  //! Set mechanics coefficient constant zone values (for composite materials)
  //! \param id index of coefficient
  //! \param binPath path to BIN file
  void setCoeffCompositeZoneFromBin(size_t id, const std::filesystem::path& binPath);
  //! Set mechanics coefficient constant value (for composite materials)
  //! \param coefficients values of coefficients
  void setCoeffComposites(const std::vector<double>& coefficents) {
    coeffComposites.resize(0);
    for (size_t i = 0; i < coefficents.size(); i++) {
      coeffComposites.push_back(CoeffComposite{coefficents[i]});
      coeffComposites[i].setIndex(i);
    }
  }
  //! Set name of composite coefficient (optional)
  //! \param name name
  void setCoeffCompositeName(size_t id, const std::string& name) {
    coeffComposites.at(id).setName(name);
  }

  //! \return reference to mechanics coefficient
  //! \param id index of coefficient
  Coeff& coeff(int id) { return coeffs.at(id); }
  const Coeff& coeff(int id) const { return coeffs.at(id); }
  //! \return reference to diffusion coefficient
  //! \param id index of coefficient
  CoeffK& coeffK(int id) { return coeffKs.at(id); }
  const CoeffK& coeffK(int id) const { return coeffKs.at(id); }
  //! \return reference to mechanics coefficient (value in composite)
  //! \param id index of coefficient
  CoeffComposite& coeffComposite(int id) { return coeffComposites.at(id); }
  const CoeffComposite& coeffComposite(int id) const { return coeffComposites.at(id); }

  //! \return the number of mechanics coefficient
  size_t numberCoeff() const { return coeffs.size(); }
  //! \return the number of diffusion coefficient
  size_t numberCoeffK() const { return coeffKs.size(); }
  //! \return the number of diffusion coefficient (composite)
  size_t numberCoeffComposite() const { return coeffComposites.size(); }

  //! Set the number of mechanics coefficient
  void setNumberCoeff(size_t n) {
    coeffs.resize(n);
    for (size_t i = 0; i < n; i++) coeffs[i].setIndex(i);
  }
  //! Set the number of diffusion coefficient
  void setNumberCoeffK(size_t n) {
    coeffKs.resize(n);
    for (size_t i = 0; i < n; i++) coeffKs[i].setIndex(i);
  }
  //! Set the number of mechanics coefficient (composite)
  void setNumberCoeffComposite(size_t n) {
    coeffComposites.resize(n);
    for (size_t i = 0; i < n; i++) coeffComposites[i].setIndex(i);
  }

  //! \return the number of zones
  size_t numberZones() const { return zones_.size(); }

  //! define a zone
  //! \param zone voxel grid coordinates
  //! \param coeffs mechanics coefficients
  //! \param coeffKs diffusion coefficients
  void addZone(const Zone& zone, const std::vector<double>& coeffs = {},
               const std::vector<double>& coeffKs = {},
               const std::vector<double>& coeffComposites = {});

  //! \return zone list
  const std::vector<Zone>& zones() const { return zones_; }
  std::vector<Zone>& zones() { return zones_; }

  //! \return number an internal variables
  size_t numberIntVars() { return intvars_.size(); }
  //! set number of internal variables
  void setNumberIntVars(size_t n) { intvars_.resize(n); }
  //! Add an internal variable
  //! \param field initial values of varaible
  void addIntVar(Field<double>&& intvar);
  void addIntVar(const Field<double>& intvar);
  void addIntVar(double value);

  //! Get internal variable
  //! \param id index of variable
  const IntVar& intVar(int id) const { return intvars_.at(id); }
  IntVar& intVar(int id) { return intvars_.at(id); }

  //! Set name of internal variable
  //! \param name name
  void setIntVarName(size_t id, const std::string& name) { intvars_.at(id).setName(name); }

  // Used for file generation
  void setIndex(int index) { id = index; }

  //! Interphase material appear nowhere 'pure'.
  bool interphase = false;

 private:
  int id = -1;
  size_t nbZones = 0;
  std::string law, lib, lawK, libK;
  // coefficients
  std::vector<Coeff> coeffs;
  std::vector<CoeffK> coeffKs;
  std::vector<CoeffComposite> coeffComposites;
  std::vector<Zone> zones_;
  std::vector<IntVar> intvars_;
};

}  // namespace amitex

#endif  // _AMITEX_MATERIAL_HEADER_
