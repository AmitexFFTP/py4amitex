#ifndef _AMITEX_ALGORITHM_HEADER_
#define _AMITEX_ALGORITHM_HEADER_

#include <optional>
#include <variant>

#include "amitex/private/type_element.hpp"
#include "amitex/private/value_element.hpp"

//! \file algorithm.hpp

namespace amitex {

//! Convergence accelaration setting(s)
class ConvergenceAcceleration {
 public:
  ConvergenceAcceleration() = default;
  ConvergenceAcceleration(bool value) : value{value} {};
  ConvergenceAcceleration(bool value, std::optional<int> modACV = std::nullopt)
      : value{value}, modACV{modACV} {};
  ConvergenceAcceleration(const std::variant<bool, ConvergenceAcceleration>& value);
  ConvergenceAcceleration& operator=(bool value) {
    this->value = value;
    return *this;
  }
  operator bool() const { return value.value(); }
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Convergence_Acceleration");
    if (value)
      ret.setAttribute("Value", value.value());
    else
      ret.setAttribute("Value", "Default");
    if (modACV) ret.setAttribute("modACV", modACV.value());
    return ret;
  }

  //! number of iterations between two convergence accelerations
  std::optional<int> modACV;
  std::optional<bool> value;
};

//! Force convergence
class ConvergenceForced {
 public:
  ConvergenceForced() = default;
  ConvergenceForced(bool value, int nitCVFor = 0, int nCVFor = 0)
      : value{value}, nitCVFor{nitCVFor}, nCVFor{nCVFor} {};
  ConvergenceForced& operator=(bool value) {
    this->value = value;
    return *this;
  }
  operator bool() const { return value; }
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Convergence_Forced");
    ret.setAttribute("Value", value);
    ret.setAttribute("Nit_cvfor", nitCVFor);
    ret.setAttribute("Ncvfor", nCVFor);
    return ret;
  }

  bool value;
  //! TODO: Document
  int nitCVFor = 0;
  //! TODO: Document
  int nCVFor = 0;
};

//! Substepping
class Substepping {
 public:
  Substepping() = default;
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Substepping");
    ret.setAttribute("Nitermax2", nitermax2);
    ret.setAttribute("Geom_ratio", geomRatio);
    ret.setAttribute("Nsub", nsub);
    ret.setAttribute("Depth", depth);
    return ret;
  }

  //! initial number of iterations before supstepping
  int nitermax2 = 1;
  //! after each substepping Nitermax2 = Geom_ratio * Nitermax2
  double geomRatio = 1.0;
  //! number of stubsteps when activating a new substepping
  int nsub = 1;
  //! maximum number of impricated substeppings
  int depth = 1;
};

//! Main algorithm parameters
class Algorithm {
 public:
  Algorithm() = default;
  Algorithm(const std::string& type,
            const std::variant<bool, ConvergenceAcceleration>& convergenceAcceleration,
            std::optional<double> convergenceCriterion = std::nullopt,
            std::optional<int> nitermax = std::nullopt,
            std::optional<double> convergenceCriterionSmacro = std::nullopt,
            std::optional<double> convergenceCriterionCompatibility = std::nullopt,
            std::optional<int> nitermin = std::nullopt,
            std::optional<int> niterminACV = std::nullopt,
            const std::optional<std::string>& initialize = std::nullopt,
            const std::optional<Substepping>& substepping = std::nullopt,
            const std::optional<ConvergenceForced>& ConvergenceForced = std::nullopt);

  //! Type ("Dafault" or "Basic_Scheme")
  std::string type = "Default";

  //! convergence criterion (<1.e-3)
  Value<double> convergenceCriterion{"Convergence_Criterion"};
  //! Toggle use of convergence acceleration
  ConvergenceAcceleration convergenceAcceleration;
  //! max number of iteration
  Value<int> nitermax{"Nitermax"};
  //! Convergence criterion for macroscropic applied stress
  //! default value is the one used for convergenceCriterion
  Value<double> convergenceCriterionSmacro{"Convergence_Criterion_Smacro"};
  //!  compatibility criterion
  Value<double> convergenceCriterionCompatibility{"Convergence_Criterion_Compatibility"};
  //! minimum number of iterations (0 or 1)
  Value<int> nitermin{"Nitermin"};
  //! minimum number of iterations (0 or 1) after each accelerated solution
  Value<int> niterminACV{"Nitermin_acv"};
  //! Pre-step initialization ("default" or "previous")
  Value<std::string> initialize{"Initialize"};

  //! substepping
  std::optional<Substepping> substepping;

  //! Force convergence
  std::optional<ConvergenceForced> convergenceForced;

  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Algorithm");
    ret.setAttribute("Type", type);
    ret.appendChild(convergenceCriterion.toXMLElement(doc));
    if (convergenceAcceleration.value) ret.appendChild(convergenceAcceleration.toXMLElement(doc));
    if (nitermax.value) ret.appendChild(nitermax.toXMLElement(doc));
    if (convergenceCriterionSmacro.value)
      ret.appendChild(convergenceCriterionSmacro.toXMLElement(doc));
    if (convergenceCriterionCompatibility.value)
      ret.appendChild(convergenceCriterionCompatibility.toXMLElement(doc));
    if (nitermin.value) ret.appendChild(nitermin.toXMLElement(doc));
    if (niterminACV.value) ret.appendChild(niterminACV.toXMLElement(doc));
    if (initialize.value) ret.appendChild(initialize.toXMLElement(doc));
    if (substepping) ret.appendChild(substepping.value().toXMLElement(doc));
    if (convergenceForced) ret.appendChild(convergenceForced.value().toXMLElement(doc));
    return ret;
  }
};

class Diffusion {
 public:
  Diffusion() = default;
  Diffusion(const std::string& filter, bool stationary)
      : filter{"Filter", filter}, stationary{"Stationary", stationary} {}
  Type filter{"Filter"};
  Value<bool> stationary{"Stationary", true};

  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Diffusion");
    ret.appendChild(filter.toXMLElement(doc));
    ret.appendChild(stationary.toXMLElement(doc));
    return ret;
  }
};

class SmallPerturbations {
 public:
  SmallPerturbations() = default;
  SmallPerturbations(bool value) : value{value} {};
  SmallPerturbations(bool value, const std::string& displacementGradient)
      : value{value}, displacementGradient{displacementGradient} {};
  SmallPerturbations& operator=(bool value) {
    this->value = value;
    return *this;
  }
  operator bool() const { return value; }
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Small_Perturbations");
    ret.setAttribute("Value", value);
    if (!displacementGradient.empty())
      ret.setAttribute("Displacement_Gradient", displacementGradient);
    return ret;
  }

  //! Toggle non symmetrized displacement gradients (="nsysm" or ommited)
  std::string displacementGradient;

  bool value;
};

class Mechanics {
 public:
  Mechanics() = default;
  Mechanics(const std::string& filter, std::variant<SmallPerturbations, bool> smallPerturbations);
  Type filter{"Filter"};

  SmallPerturbations smallPerturbations = true;
  // Toggle non-symmetric reference material behavior for finite strains
  Value<bool> C0Sym{"C0Sym"};

  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Mechanics");
    ret.appendChild(filter.toXMLElement(doc));
    ret.appendChild(smallPerturbations.toXMLElement(doc));
    if (C0Sym.value) ret.appendChild(C0Sym.toXMLElement(doc));
    return ret;
  }
};

}  // namespace amitex

#endif  // _AMITEX_ALGORITHM_HEADER_
