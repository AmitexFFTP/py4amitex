#ifndef _AMITEX_OUTPUT_HEADER_
#define _AMITEX_OUTPUT_HEADER_

#include "amitex/private/input_element.hpp"
#include "amitex/private/list_element.hpp"

namespace amitex {

//! Control output of stress and strain
class VtkStressStrain {
 public:
  VtkStressStrain() = default;
  VtkStressStrain(int stress, int strain) : stress{stress}, strain{strain} {}
  const char* xmlTag() const { return "vtk_StressStrain"; }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const {}

  //! Output stress to VTK file(s) (0 or 1)
  int stress = 0;
  //! Output strain to VTK file(s) (0 or 1)
  int strain = 0;
};

//! Control output of diffusion flux and gradient
class VtkFluxDGradD {
 public:
  VtkFluxDGradD() = default;
  VtkFluxDGradD(int fluxd, int gradd) : fluxd{fluxd}, gradd{gradd} {}
  const char* xmlTag() const { return "vtk_FluxDGradD"; }
  bool xmlHasBody() const { return false; }
  void xmlWriteAttributes(std::ostream& stream) const;
  void xmlWriteInner(std::ostream& stream) const {}

  //! Output flux to VTK file(s) (0 or 1)
  int fluxd = 0;
  //! Output gradient to VTK file(s) (0 or 1)
  int gradd = 0;
};

//! Output parametrization
class Output {
 public:
  class Zone {
   public:
    Zone() = default;
    //! \param numM material index
    Zone(size_t numM) : numM{numM} {};
    const char* xmlTag() const { return "Zone"; }
    bool xmlHasBody() const;
    void xmlWriteAttributes(std::ostream& stream) const;
    void xmlWriteInner(std::ostream& stream) const;

    //! Set the list of internal variables to be printed
    //! \param list list of indices of the internal variables
    void setVarIntList(const std::vector<size_t>& list);

    size_t numM;

   private:
    List<size_t> varIntList{"VarIntList"};
  };

  const char* xmlTag() const { return "Output"; }
  bool xmlHasBody() const { return true; }
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const;

  //! Add zones of a material to .zstd output
  void addZone(const Zone& zone) { zones.push_back(zone); }
  void addZone(Zone&& zone) { zones.push_back(zone); };
  //! Add zones of a material to .zstd output
  //! \param numM index of material
  //! \param intVarList list of internal variable indices
  void addZone(size_t numM, const std::vector<size_t>& intVarList = {}) {
    Zone zone{numM};
    zone.setVarIntList(intVarList);
    zones.push_back(zone);
  }

  //! Control output of stress and strain
  void setVtkStressStrain(int stress, int strain) {
    vtkStressStrain = VtkStressStrain{stress, strain};
  }

  //! Control output of diffusion flux and gradient
  void setVtkFluxDGradD(int fluxD, int gradD) { vtkFluxDGradD = VtkFluxDGradD{fluxD, gradD}; }

  //! Set the internal variables for field output for a given material
  void addVtkIntVarList(size_t numM, const std::vector<size_t>& list) {
    VtkIntVarList ivl{numM};
    for (auto idx : list) ivl.add(idx + 1);
    intVarList.push_back(ivl);
  }

  std::optional<VtkFluxDGradD> vtkFluxDGradD;
  std::optional<VtkStressStrain> vtkStressStrain;

 private:
  class VtkIntVarList : public List<size_t> {
   public:
    VtkIntVarList(size_t numM) : numM{numM}, List{"vtk_IntVarList"} {};
    size_t numM;
    void xmlWriteAttributes(std::ostream& stream) const;
  };

  std::vector<Zone> zones;
  std::vector<VtkIntVarList> intVarList;
};

}  // namespace amitex

#endif  // _AMITEX_OUTPUT_HEADER_