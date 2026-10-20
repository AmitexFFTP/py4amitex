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

Mechanics::Mechanics(const std::string& filter,
                     std::variant<SmallPerturbations, bool> smallPerturbations) {
  this->filter = filter;
  if (std::holds_alternative<bool>(smallPerturbations)) {
    this->smallPerturbations = std::get<bool>(smallPerturbations);
  } else {
    this->smallPerturbations = std::get<SmallPerturbations>(smallPerturbations);
  }
}

}  // namespace amitex
