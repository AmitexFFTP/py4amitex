#ifndef _AMITEX_ALGORITHM_HEADER_
#define _AMITEX_ALGORITHM_HEADER_

#include <optional>
#include <variant>

#include "amitex/private/input_element.hpp"
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
  const char* xmlTag() const { return "Convergence_Acceleration"; }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const {}

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
  operator bool() const { return false; }
  const char* xmlTag() const { return "Convergence_Forced"; }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const {}

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
  const char* xmlTag() const { return "Substepping"; }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const {}

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
            const std::optional<Substepping>& substepping = std::nullopt,
            const std::optional<ConvergenceForced>& ConvergenceForced = std::nullopt);

  //! Type ("Dafault" or "Basic_Scheme")
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
  std::optional<Substepping> substepping;

  //! Force convergence
  std::optional<ConvergenceForced> convergenceForced;

  const char* xmlTag() const { return "Algorithm"; }
  bool xmlHasBody() const { return true; }
  void xmlWriteInner(std::ostream& stream) const;
  void xmlWriteAttributes(std::ostream& stream) const { writeXMLAttributes(stream, "Type", type); }
};

class Diffusion {
 public:
  Diffusion() = default;
  Diffusion(const std::string& filter, bool stationary)
      : filter{"Filter", filter}, stationary{"Stationary", stationary} {}
  Type filter{"Filter"};
  Value<bool> stationary{"Stationary", true};

  const char* xmlTag() const { return "Diffusion"; }
  bool xmlHasBody() const { return true; }
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const {
    writeXML(stream, filter);
    writeXML(stream, stationary);
  }
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
  const char* xmlTag() const { return "Small_Perturbations"; }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const {}

  //! Toggle non symmetrized displacement gradients (="nsysm" or ommited)
  std::string displacementGradient = "";

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

  const char* xmlTag() const { return "Mechanics"; }
  bool xmlHasBody() const { return true; }
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const;
};

}  // namespace amitex

#endif  // _AMITEX_ALGORITHM_HEADER_
