#ifndef _AMITEX_LOADING_HEADER_
#define _AMITEX_LOADING_HEADER_

#include <optional>
#include <utility>

#include "amitex/errors.hpp"
#include "amitex/input/init_load_ext.hpp"
#include "amitex/input/user_interruption.hpp"

#include "amitex/private/input_element.hpp"
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
  const char* xmlTag() const { return "Loading"; }
  bool xmlHasBody() const { return true; }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const;
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

 private:
  class MechanicsDriver {
   public:
    MechanicsDriver(AxisIndexPair component) : component{component} {
      tag = std::string{AxisStrings[component.first]} + AxisStrings[component.second];
    }
    const char* xmlTag() const { return tag.c_str(); }
    bool xmlHasBody() const { return false; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const {}

    AxisIndexPair component;
    MechanicDriving driving = MechanicDriving::Strain;
    Evolution evolution = Evolution::Constant;
    double value = 0.0;
    std::optional<double> dirstress;
    std::string tag;
  };

  class DiffusionDriver {
   public:
    DiffusionDriver(int component) : component{component} {
      tag = std::string{AxisStrings[component]} + "0";
    }
    const char* xmlTag() const { return tag.c_str(); }
    bool xmlHasBody() const { return false; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const {}

    int component;
    DiffusionDriving driving = DiffusionDriving::Gradient;
    Evolution evolution = Evolution::Constant;
    double value = 0.0;
    std::string tag;
  };

  class DirStress_ {
   public:
    DirStress_() = default;
    DirStress_(DirStress dirstress) : dirstress{dirstress} {}
    const char* xmlTag() const { return "DirStress"; }
    bool xmlHasBody() const { return 0; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const {}

   private:
    DirStress dirstress = Cauchy;
  };
  //! Component of a gradient of gradient of displacement
  class GradGradU {
   public:
    GradGradU(AxisIndexPair ii, int j);
    const char* xmlTag() const { return tag_.c_str(); }
    bool xmlHasBody() const { return 0; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const {}

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
    const char* xmlTag() const { return "Time_Discretization"; }
    bool xmlHasBody() const { return false; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const {}

   private:
    std::string discretization = "Linear";
    size_t nincr = 1;
    double tfinal = -1.0;
  };
  class Temperature {
   public:
    Temperature() = default;
    const char* xmlTag() const { return "T"; }
    bool xmlHasBody() const { return false; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const {}
    Evolution evolution;
    double value = 0;
  };
  class Param {
   public:
    Param() = default;
    const char* xmlTag() const { return "Param"; }
    bool xmlHasBody() const { return false; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const {}
    size_t index;
    Evolution evolution;
    double value = 0;
  };
  class OutputNumber {
   public:
    OutputNumber(const char* tag) : tag_{tag} {}
    OutputNumber(int number) : number{number} {};
    const char* xmlTag() const { return tag_; }
    bool xmlHasBody() const { return false; }
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const {}
    int number = -1;

   private:
    const char* tag_;
  };

  using TimeList = List<double>;
  using OutputVtkList = List<int>;

  int id = -1;
  DirStress_ dirstress_;
  TimeDiscretisation tdisc;
  bool userTimeList = false;
  TimeList tlist{"Time_List"};
  std::vector<DiffusionDriver> diffu_drivers;
  std::vector<MechanicsDriver> meca_drivers;
  OutputVtkList outputVtkList{"Output_vtkList"};
  std::optional<Temperature> temperature;
  std::vector<Param> params;
  OutputNumber outputCell{"Output_cell"};
  OutputNumber outputZone{"Output_zone"};
  std::vector<UserInterruption> userItpts;
  std::vector<GradGradU> gradGradUDrivers;
};
};  // namespace amitex

#endif  // _AMITEX_LOADING_HEADER_
