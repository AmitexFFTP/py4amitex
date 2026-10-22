/*
SPDX-License-Identifier: LGPL-3.0-only

(C) copyright 2026 of CEA

author: Guillaume Bareigts <guillaume.bareigts@cea.fr>, CEA
*/
#ifndef _AMITEX_ALGORITHM_HEADER_
#define _AMITEX_ALGORITHM_HEADER_

#include <optional>
#include <variant>

#include "amitex/input/common.hpp"

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
  std::optional<int> modACV = std::nullopt;
  std::optional<bool> value = std::nullopt;
};

//! Force convergence
class ConvergenceForced {
 public:
  ConvergenceForced() : ConvergenceForced(0.0) {}
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
  static Ptr<Substepping> create() { return makePtr<Substepping>(Substepping{}); }

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

 private:
  Substepping() = default;
};

//! Main algorithm parameters
class Algorithm {
 public:
  static Ptr<Algorithm> create(
      const std::string& type,
      const std::variant<bool, ConvergenceAcceleration>& convergenceAcceleration,
      std::optional<double> convergenceCriterion = std::nullopt,
      std::optional<int> nitermax = std::nullopt,
      std::optional<double> convergenceCriterionSmacro = std::nullopt,
      std::optional<double> convergenceCriterionCompatibility = std::nullopt,
      std::optional<int> nitermin = std::nullopt, std::optional<int> niterminACV = std::nullopt,
      const std::optional<std::string>& initialize = std::nullopt,
      Ptr<Substepping> substepping = nullptr,
      const std::optional<ConvergenceForced>& ConvergenceForced = std::nullopt) {
    return makePtr<Algorithm>(Algorithm{type, convergenceAcceleration,

                                        convergenceCriterion, nitermax, convergenceCriterionSmacro,
                                        convergenceCriterionCompatibility, nitermin, niterminACV,
                                        initialize, substepping, ConvergenceForced});
  }
  static Ptr<Algorithm> createDefault() { return makePtr<Algorithm>(Algorithm{}); }

  //! Type ("Default" or "Basic_Scheme")
  std::string type;

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
  Ptr<Substepping> substepping = nullptr;

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
    if (substepping) ret.appendChild(substepping->toXMLElement(doc));
    if (convergenceForced) ret.appendChild(convergenceForced.value().toXMLElement(doc));
    return ret;
  }

 private:
  Algorithm() : Algorithm{"Default", true} {};
  Algorithm(const std::string& type,
            const std::variant<bool, ConvergenceAcceleration>& convergenceAcceleration,
            std::optional<double> convergenceCriterion = std::nullopt,
            std::optional<int> nitermax = std::nullopt,
            std::optional<double> convergenceCriterionSmacro = std::nullopt,
            std::optional<double> convergenceCriterionCompatibility = std::nullopt,
            std::optional<int> nitermin = std::nullopt,
            std::optional<int> niterminACV = std::nullopt,
            const std::optional<std::string>& initialize = std::nullopt,
            Ptr<Substepping> substepping = nullptr,
            const std::optional<ConvergenceForced>& ConvergenceForced = std::nullopt);
};

class Diffusion {
 public:
  static Ptr<Diffusion> create(const std::string& filter, bool stationary) {
    return makePtr<Diffusion>(Diffusion{filter, stationary});
  }
  static Ptr<Diffusion> createDefault() { return makePtr<Diffusion>(Diffusion{}); }
  Type filter{"Filter"};
  Value<bool> stationary{"Stationary", true};

  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Diffusion");
    ret.appendChild(filter.toXMLElement(doc));
    ret.appendChild(stationary.toXMLElement(doc));
    return ret;
  }

 private:
  Diffusion() = default;
  Diffusion(const std::string& filter, bool stationary)
      : filter{"Filter", filter}, stationary{"Stationary", stationary} {}
};

class SmallPerturbations {
 public:
  SmallPerturbations() : SmallPerturbations{true} {};
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
  std::string displacementGradient = "";

  bool value;
};

class Mechanics {
 public:
  static Ptr<Mechanics> create(const std::string& filter,
                               std::variant<SmallPerturbations, bool> smallPerturbations) {
    return makePtr<Mechanics>(Mechanics{filter, smallPerturbations});
  }
  static Ptr<Mechanics> createDefault() { return makePtr<Mechanics>(Mechanics{}); }

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
  
  private:
   Mechanics() = default;
   Mechanics(const std::string& filter, std::variant<SmallPerturbations, bool> smallPerturbations);
};

}  // namespace amitex

#endif  // _AMITEX_ALGORITHM_HEADER_
