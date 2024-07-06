#include "amitex/input/material.hpp"

#include "amitex/errors.hpp"

#include "amitex/io.hpp"

namespace amitex {

static void resizeAndSet(std::vector<double>& vec, size_t index, double value) {
  if (index >= vec.size()) vec.resize(index + 1);
  vec[index] = value;
}

void Material::addZone(const Zone& zone, const std::vector<double>& coeffs,
                       const std::vector<double>& coeffKs,
                       const std::vector<double>& coeffComposites) {
  size_t nbZones = numberZones() + 1;
  zones_.push_back(zone);
  interphase = false;
  if (!coeffs.empty()) {
    if (numberCoeff() != coeffs.size())
      throw InputError{
          "number coeffs in addZone et Materials do not correspond, maybe call setNumberCoeff "
          "beforehand"};
    for (size_t i = 0; i < numberCoeff(); i++) {
      resizeAndSet(coeff(i).zoneValues(), nbZones - 1, coeffs[i]);
    }
  } else if (numberCoeff() > 0) {
    if (coeff(0).zoneValues().size() > 0) {
      throw InputError{
          "addZone: Once the coeff of the 1st zone is set, those of other zones must be set"};
    }
  }
  if (!coeffKs.empty()) {
    if (numberCoeffK() != coeffKs.size())
      throw InputError{
          "number coeffKs in addZone et Materials do not correspond, maybe call setNumberK "
          "beforehand"};
    for (size_t i = 0; i < numberCoeffK(); i++) {
      resizeAndSet(coeffK(i).zoneValues(), nbZones - 1, coeffKs[i]);
    }
  } else if (numberCoeffK() > 0) {
    if (coeffK(0).zoneValues().size() > 0) {
      throw InputError{
          "addZone: Once the coeff of the 1st zone is set, those of other zones must be set"};
    }
  }
  if (!coeffComposites.empty()) {
    if (numberCoeffComposite() != coeffComposites.size())
      throw InputError{
          "number coeffComposites in addZone et Materials do not correspond, maybe call "
          "setNumberCoeffComposite beforehand"};
    for (size_t i = 0; i < numberCoeffComposite(); i++) {
      resizeAndSet(coeffComposite(i).zoneValues(), nbZones - 1, coeffComposites[i]);
    }
  } else if (numberCoeffComposite() > 0) {
    if (coeffComposite(0).zoneValues().size() > 0) {
      throw InputError{
          "addZone: Once the coeff of the 1st zone is set, those of other zones must be set"};
    }
  }
}

void Material::addIntVar(Field<double>&& intvar) {
  intvars_.push_back(IntVar{intvar});
  intvars_.back().setIndex(intvars_.size() - 1);
}
void Material::addIntVar(const Field<double>& intvar) {
  intvars_.push_back(IntVar{intvar});
  intvars_.back().setIndex(intvars_.size() - 1);
}
void Material::addIntVar(double value) {
  intvars_.push_back(IntVar{value});
  intvars_.back().setIndex(intvars_.size() - 1);
}

void Material::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "numM", id + 1);
  if (!law.empty()) {
    writeXMLAttributes(stream, "Law", law);
    writeXMLAttributes(stream, "Lib", lib);
  }
  if (!lawK.empty()) {
    writeXMLAttributes(stream, "LawK", lawK);
    writeXMLAttributes(stream, "LibK", libK);
  }
}

void Material::xmlWriteInner(std::ostream& stream) const {
  for (const auto& coeff : coeffs) {
    writeXML(stream, coeff);
  }
  for (const auto& coeffK : coeffKs) {
    writeXML(stream, coeffK);
  }
  for (const auto& coeffC : coeffComposites) {
    writeXML(stream, coeffC);
  }
  for (const auto& ivar : intvars_) {
    writeXML(stream, ivar);
  }
}

void Material::setCoeffZoneFromBin(size_t id, const std::string& binPath) {
  std::vector<double> data;
  readBin(binPath, data);
  coeffs.at(id) = std::move(Coeff{std::move(data)});
  coeffs.at(id).setIndex(id);
}

void Material::setCoeffKZoneFromBin(size_t id, const std::string& binPath) {
  std::vector<double> data;
  readBin(binPath, data);
  coeffKs.at(id) = std::move(CoeffK{std::move(data)});
  coeffKs.at(id).setIndex(id);
}

void Material::setCoeffCompositeZoneFromBin(size_t id, const std::string& binPath) {
  std::vector<double> data;
  readBin(binPath, data);
  coeffComposites.at(id) = std::move(CoeffComposite{std::move(data)});
  coeffComposites.at(id).setIndex(id);
}

}  // namespace amitex
