#include "amitex/input/algorithm.hpp"

namespace amitex {

ConvergenceAcceleration::ConvergenceAcceleration(
    const std::variant<bool, ConvergenceAcceleration>& value) {
  if (std::holds_alternative<bool>(value)) {
    this->value = std::get<0>(value);
  } else {
    *this = std::get<1>(value);
  }
}

void ConvergenceAcceleration::xmlWriteAttributes(std::ostream& stream) const {
  if (value)
    writeXMLAttributes(stream, "Value", value.value());
  else
    writeXMLAttributes(stream, "Value", "Default");
  if (modACV) writeXMLAttributes(stream, "modACV", modACV.value());
}

void ConvergenceForced::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Value", value);
  writeXMLAttributes(stream, "Nit_cvfor", nitCVFor);
  writeXMLAttributes(stream, "Ncvfor", nCVFor);
}

void Substepping::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Nitermax2", nitermax2);
  writeXMLAttributes(stream, "Geom_ratio", geomRatio);
  writeXMLAttributes(stream, "Nsub", nsub);
  writeXMLAttributes(stream, "Depth", depth);
}

Algorithm::Algorithm(const std::string& type,
                     const std::variant<bool, ConvergenceAcceleration>& convergenceAcceleration,
                     std::optional<double> convergenceCriterion, std::optional<int> nitermax,
                     std::optional<double> convergenceCriterionSmacro,
                     std::optional<double> convergenceCriterionCompatibility,
                     std::optional<int> nitermin, std::optional<int> niterminACV,
                     const std::optional<std::string>& initialize, Ptr<Substepping> substepping,
                     const std::optional<ConvergenceForced>& convergenceForced)
    : type{type},
      convergenceAcceleration{convergenceAcceleration},
      convergenceCriterion{"Convergence_Criterion", convergenceCriterion},
      nitermax{"Nitermax", nitermax},
      convergenceCriterionSmacro{"Convergence_Criterion_Smacro", convergenceCriterionSmacro},
      convergenceCriterionCompatibility{"Convergence_Criterion_Compatibility",
                                        convergenceCriterionCompatibility},
      nitermin{"Nitermin", nitermin},
      niterminACV{"Nitermin_acv", niterminACV},
      initialize{"Initialize", initialize},
      substepping{substepping},
      convergenceForced{convergenceForced} {}

void Algorithm::xmlWriteInner(std::ostream& stream) const {
  writeXML(stream, convergenceCriterion);
  if (convergenceAcceleration.value) writeXML(stream, convergenceAcceleration);
  if (nitermax.value) writeXML(stream, nitermax);
  if (convergenceCriterionSmacro.value) writeXML(stream, convergenceCriterionSmacro);
  if (convergenceCriterionCompatibility.value) writeXML(stream, convergenceCriterionCompatibility);
  if (nitermin.value) writeXML(stream, nitermin);
  if (niterminACV.value) writeXML(stream, niterminACV);
  if (initialize.value) writeXML(stream, initialize);
  if (substepping) writeXML(stream, *substepping);
  if (convergenceForced) writeXML(stream, convergenceForced.value());
}

void SmallPerturbations::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Value", value);
  if (!displacementGradient.empty())
    writeXMLAttributes(stream, "Displacement_Gradient", displacementGradient);
}

Mechanics::Mechanics(const std::string& filter,
                     std::variant<SmallPerturbations, bool> smallPerturbations) {
  this->filter = filter;
  if (std::holds_alternative<bool>(smallPerturbations)) {
    this->smallPerturbations = std::get<bool>(smallPerturbations);
  } else {
    this->smallPerturbations = std::get<SmallPerturbations>(smallPerturbations);
  }
}

void Mechanics::xmlWriteInner(std::ostream& stream) const {
  writeXML(stream, filter);
  writeXML(stream, smallPerturbations);
  if (C0Sym.value) writeXML(stream, C0Sym);
}

}  // namespace amitex
