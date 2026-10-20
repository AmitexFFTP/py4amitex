#ifndef _AMITEX_LOADING_HEADER_
#define _AMITEX_LOADING_HEADER_

#include <algorithm>
#include <optional>
#include <string_view>
#include <utility>

#include "amitex/input/user_interruption.hpp"

#include "amitex/private/list_element.hpp"

namespace amitex {

//! pair of axis indices
using AxisIndexPair = std::pair<int, int>;

//! Loading axis pair and axis
//! \note only finite strain needs YX,ZX, ZY
class Component {
 public:
  static constexpr AxisIndexPair XX = {0, 0};
  static constexpr AxisIndexPair YY = {1, 1};
  static constexpr AxisIndexPair ZZ = {2, 2};
  static constexpr AxisIndexPair XY = {0, 1};
  static constexpr AxisIndexPair XZ = {0, 2};
  static constexpr AxisIndexPair YZ = {1, 2};
  static constexpr AxisIndexPair YX = {1, 0};
  static constexpr AxisIndexPair ZX = {2, 0};
  static constexpr AxisIndexPair ZY = {2, 1};
  static constexpr int X = 0;
  static constexpr int Y = 1;
  static constexpr int Z = 2;
};
extern const char* AxisStrings[3];

//! Types of driving for mechanical problems
enum class MechanicDriving { Strain = 0, Stress = 1 };

//! Types of driving for difffusion problems
enum class DiffusionDriving { Gradient = 0, Flux = 1 };

//! Types of loading evolution
enum class Evolution { Constant = 0, Linear = 1 };  // External ?

//! Type of stress tensor
enum DirStress { Cauchy, PK1 };

//! (partial) loading
class Loading {
 public:
  static Ptr<Loading> create() { return makePtr<Loading>(Loading{}); }

  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Loading");
    ret.setAttribute("Tag", id + 1);
    ret.appendChild(tdisc.toXMLElement(doc));
    if (userTimeList) ret.appendChild(tlist.toXMLElement(doc));
    if (outputVtkList.numberValues() > 0) ret.appendChild(outputVtkList.toXMLElement(doc));
    for (const auto& driver : diffu_drivers) {
      ret.appendChild(driver.toXMLElement(doc));
    }
    for (const auto& driver : meca_drivers) {
      ret.appendChild(driver.toXMLElement(doc));
    }
    if (any_of(meca_drivers.begin(), meca_drivers.end(),
               [](const auto& d) { return d.dirstress; })) {
      ret.appendChild(dirstress_.toXMLElement(doc));
    }
    if (temperature) ret.appendChild(temperature.value().toXMLElement(doc));
    for (const auto& param : params) ret.appendChild(param.toXMLElement(doc));
    if (outputCell.number >= 0) ret.appendChild(outputCell.toXMLElement(doc));
    if (outputZone.number >= 0) ret.appendChild(outputZone.toXMLElement(doc));
    for (const auto& driver : gradGradUDrivers) {
      ret.appendChild(driver.toXMLElement(doc));
    }
    for (const auto& userItpt : userItpts) ret.appendChild(userItpt.toXMLElement(doc));
    if (restart) ret.appendChild(restart.value().toXMLElement(doc));
    // for (const auto& restart)
    return ret;
  }
  //! Discretize time by a constant interval
  //! \param increments the number of timesteps
  //! \param tFinal final time
  void setTimeDiscretizationLinear(size_t increments, double tFinal) {
    tdisc = TimeDiscretisation{"Linear", increments, tFinal};
  };
  //! Discretize time by a supplying a list of timesteps
  //! \param increments the number of timesteps
  //! \param times pointer to the array of times
  void setTimeDiscretizationUser(size_t increments, const double* times) {
    tdisc = TimeDiscretisation{"User", increments};
    for (size_t i = 0; i < increments; i++) {
      tlist.add(times[i]);
    }
    userTimeList = true;
  };
  //! Discretize time by a supplying a list of timesteps
  //! \param times list of times
  void setTimeDiscretizationUser(const std::vector<double>& times) {
    setTimeDiscretizationUser(times.size(), times.data());
  };
  //! set a diffusion loading evolution for one component
  //! \param component
  //! \param driving
  //! \param evolution
  //! \param value value for linear evolution
  //! \param dirstress stress direction
  void setEvolution(int component, DiffusionDriving driving, Evolution evolution,
                    double value = 0.0) {
    if (component >= 3)
      throw std::runtime_error("Invalid direction for diffusion driving (only X,Y,Z)");
    int idx = -1;
    for (size_t i = 0; i < diffu_drivers.size(); i++) {
      if (diffu_drivers[i].component == component) {
        idx = i;
        break;
      }
    }
    if (idx == -1) {
      DiffusionDriver driver{component};
      diffu_drivers.push_back(driver);
      idx = diffu_drivers.size() - 1;
    }
    auto& driver = diffu_drivers[idx];
    driver.driving = driving;
    driver.evolution = evolution;
    driver.value = value;
  }
  //! set a diffusion constant loading for one component
  //! \param component
  //! \param driving
  void setConstantEvolution(int component, DiffusionDriving driving) {
    setEvolution(component, driving, Evolution::Constant);
  }
  //! set a diffusion constant loading for one component
  //! \param component
  //! \param driving
  //! \param value final loading value along `component`
  void setLinearEvolution(int component, DiffusionDriving driving, double value) {
    setEvolution(component, driving, Evolution::Linear, value);
  }
  //! set a mechanical loading evolution for one component
  //! \param component
  //! \param driving
  //! \param evolution
  //! \param value final value for linear evolution
  //! \param dirstress stress direction
  void setEvolution(AxisIndexPair component, MechanicDriving driving, Evolution evolution,
                    double value = 0.0, std::optional<double> dirstress = std::nullopt) {
    int idx = -1;
    for (size_t i = 0; i < meca_drivers.size(); i++) {
      if (meca_drivers[i].component == component) {
        idx = i;
        break;
      }
    }
    if (idx == -1) {
      MechanicsDriver driver{component};
      meca_drivers.push_back(driver);
      idx = meca_drivers.size() - 1;
    }
    auto& driver = meca_drivers[idx];
    driver.driving = driving;
    driver.evolution = evolution;
    driver.value = value;
    driver.dirstress = dirstress;
  }
  //! set a mechanical constant loading for one component
  //! \param component
  //! \param driving
  //! \param dirstress stress direction
  void setConstantEvolution(AxisIndexPair component, MechanicDriving driving,
                            std::optional<double> dirstress = std::nullopt) {
    setEvolution(component, driving, Evolution::Constant, 0.0, dirstress);
  }
  //! set a mechanical constant loading for one component
  //! \param component
  //! \param driving
  //! \param value final loading value along `component`
  //! \param dirstress stress direction
  void setLinearEvolution(AxisIndexPair component, MechanicDriving driving, double value,
                          std::optional<double> dirstress = std::nullopt) {
    setEvolution(component, driving, Evolution::Linear, value, dirstress);
  }
  //! Set the constant stress direction behavior for finite strains
  //! \param dirStress type of stress tensor
  void setDirStress(DirStress dirStress) { dirstress_ = DirStress{dirStress}; };

  //! Set the index (or tag)
  void setIndex(int index) { id = index; }

  //! Set the list of increment numbers where to ouput
  //! \param increments list of increment numbers
  void setOutputVtkList(const std::vector<int>& increments) {
    for (int incr : increments) outputVtkList.add(incr);
  };

  //! set a the evolution of an external loading parameter
  //! \param index index of parameter (defined in \ref InitLoadExt)
  //! \param evolution
  //! \param value value for linear evolution
  void setParamEvolution(size_t index, Evolution evolution, double value = 0.0);

  //! set a the evolution of the temperature
  //! \param index index of parameter (defined in \ref InitLoadExt)
  //! \param evolution
  //! \param value value for linear evolution
  void setTemperatureEvolution(Evolution evolution, double value = 0.0);

  //! Set the number of increments where per-unit cell quantities are output
  void setOutputCell(int number) { outputCell.number = number; }

  //! Set the number of increments where zone quantities are output
  void setOutputZone(int number) { outputZone.number = number; }

  //! Set the evolution and value of a component grad_i grad_j U_k of an applied strain gradient
  //! (bending/twisting beam) component
  //! \param ij directions of gradients
  //! \param k axis of displacement
  //! \param evolution Type of evolution
  //! \param value Value for linear evolution
  //! \todo precise the meaning of components
  void setGradGradU(AxisIndexPair ij, int k, Evolution evolution, double value = 0.0);

  //! Add a value for user-defined interuptions
  void addUserInterruptValue(double value) {
    auto itpt = UserInterruption{value};
    itpt.setIndex(userItpts.size() + 1);
    userItpts.push_back(itpt);
  }

  //! Add a list of values for user-defined interuptions
  void setUserInterruptValues(const std::vector<double>& values) {
    userItpts.resize(0);
    for (auto value : values) addUserInterruptValue(value);
  }

  //!  Save restart files every `every` increments
  //!  \param every period of saving
  void setRestart(int every);

 private:
  Loading() = default;

  class MechanicsDriver {
   public:
    MechanicsDriver(AxisIndexPair component) : component{component} {
      tag = std::string{AxisStrings[component.first]} + AxisStrings[component.second];
    }
    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement(tag);
      switch (driving) {
        case MechanicDriving::Strain:
          ret.setAttribute("Driving", "Strain");
          break;
        case MechanicDriving::Stress:

          ret.setAttribute("Driving", "Stress");
      }
      switch (evolution) {
        case Evolution::Constant:
          // Redondant (?) if dirstress is imposed
          if (!(driving == MechanicDriving::Stress && dirstress))
            ret.setAttribute("Evolution", "Constant");
          break;
        case Evolution::Linear:
          ret.setAttribute("Evolution", "Linear");
          ret.setAttribute("Value", value);
      }
      if (dirstress) ret.setAttribute("DirStress", dirstress.value());
      return ret;
    }

    AxisIndexPair component;
    MechanicDriving driving = MechanicDriving::Strain;
    Evolution evolution = Evolution::Constant;
    double value = 0.0;
    std::optional<double> dirstress = std::nullopt;
    std::string tag = "";
  };

  class DiffusionDriver {
   public:
    DiffusionDriver(int component) : component{component} {
      tag = std::string{AxisStrings[component]} + "0";
    }
    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement(tag);
      switch (driving) {
        case DiffusionDriving::Gradient:
          ret.setAttribute("Driving", "GradD");
          break;
        case DiffusionDriving::Flux:
          ret.setAttribute("Driving", "FluxD");
      }
      switch (evolution) {
        case Evolution::Constant:
          ret.setAttribute("Evolution", "Constant");
          break;
        case Evolution::Linear:
          ret.setAttribute("Evolution", "Linear");
          ret.setAttribute("Value", value);
      }
      return ret;
    }

    int component;
    DiffusionDriving driving = DiffusionDriving::Gradient;
    Evolution evolution = Evolution::Constant;
    double value = 0.0;
    std::string tag = "";
  };

  class DirStress_ {
   public:
    DirStress_() = default;
    DirStress_(DirStress dirstress) : dirstress{dirstress} {}

    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement("DirStress");
      switch (dirstress) {
        case Cauchy:
          ret.setAttribute("Type", "Cauchy");
          break;
        case PK1:
          ret.setAttribute("Type", "PK1");
          break;
      }
      return ret;
    }

   private:
    DirStress dirstress = Cauchy;
  };
  //! Component of a gradient of gradient of displacement
  class GradGradU {
   public:
    GradGradU(AxisIndexPair ii, int j);

    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement(tag_);
      switch (evolution) {
        case Evolution::Constant:
          ret.setAttribute("Evolution", "Constant");
          break;
        case Evolution::Linear:
          ret.setAttribute("Evolution", "Linear");
          ret.setAttribute("Value", value);
      }
      return ret;
    }

    AxisIndexPair tcomponent;
    int vcomponent;
    Evolution evolution = Evolution::Constant;
    double value = 0.0;

   private:
    std::string tag_;
  };

  class TimeDiscretisation {
   public:
    TimeDiscretisation() = default;
    TimeDiscretisation(const char* discretization, size_t increments, double t_final = -1.0)
        : discretization{discretization}, nincr{increments}, tfinal{t_final} {}

    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement("Time_Discretization");
      ret.setAttribute("Discretization", discretization);
      ret.setAttribute("Nincr", nincr);
      if (tfinal > 0.0) {
        ret.setAttribute("Tfinal", tfinal);
      }
      return ret;
    }

   private:
    std::string discretization = "Linear";
    size_t nincr = 1;
    double tfinal = -1.0;
  };
  class Temperature {
   public:
    Temperature() = default;

    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement("T");
      setEvolutionAttr<TDoc>(ret, evolution, value);
      return ret;
    }

    Evolution evolution{};
    double value = 0;
  };
  class Param {
   public:
    Param() = default;

    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement("Param");
      ret.setAttribute("Index", index + 1);
      setEvolutionAttr<TDoc>(ret, evolution, value);
      return ret;
    }

    size_t index = 0;
    Evolution evolution{};
    double value = 0;
  };
  class OutputNumber {
   public:
    OutputNumber(std::string_view tag) : tag_{tag} {}
    OutputNumber(std::string_view tag, int number) : tag_{tag}, number{number} {};

    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement(tag_);
      ret.setAttribute("Number", number);
      return ret;
    }
    int number = -1;

   private:
    std::string tag_;
  };
  class Restart {
   public:
    Restart() {}
    Restart(int every) : every{every} {}

    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement("Restart");
      ret.setAttribute("Every", every);
      return ret;
    }
    int every = 0;
  };

  using TimeList = List<double>;
  using OutputVtkList = List<int>;

  int id = -1;
  DirStress_ dirstress_{};
  TimeDiscretisation tdisc{};
  bool userTimeList = false;
  TimeList tlist{"Time_List"};
  std::vector<DiffusionDriver> diffu_drivers = {};
  std::vector<MechanicsDriver> meca_drivers = {};
  OutputVtkList outputVtkList{"Output_vtkList"};
  std::optional<Temperature> temperature = std::nullopt;
  std::vector<Param> params = {};
  OutputNumber outputCell{"Output_cell"};
  OutputNumber outputZone{"Output_zone"};

  template <typename TDoc>
  static void setEvolutionAttr(typename TDoc::Node elem, Evolution evolution, double value) {
    switch (evolution) {
      case Evolution::Constant:
        elem.setAttribute("Evolution", "Constant");
        break;
      case Evolution::Linear:
        elem.setAttribute("Evolution", "Linear");
        elem.setAttribute("Value", value);
    }
  }

  std::vector<UserInterruption> userItpts = {};
  std::vector<GradGradU> gradGradUDrivers = {};
  std::optional<Restart> restart;
};
};  // namespace amitex

#endif  // _AMITEX_LOADING_HEADER_
