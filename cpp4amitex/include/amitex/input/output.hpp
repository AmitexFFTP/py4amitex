#ifndef _AMITEX_OUTPUT_HEADER_
#define _AMITEX_OUTPUT_HEADER_

#include "amitex/private/list_element.hpp"

namespace amitex {

//! Control output of stress and strain
class VtkStressStrain {
 public:
  VtkStressStrain() = default;
  VtkStressStrain(int stress, int strain) : stress{stress}, strain{strain} {}
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("vtk_StressStrain");
    ret.setAttribute("Strain", strain);
    ret.setAttribute("Stress", stress);
    return ret;
  }

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
  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("vtk_FluxDGradD");
    ret.setAttribute("FluxD", fluxd);
    ret.setAttribute("GradD", gradd);
    return ret;
  }

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
    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = doc.createElement("Zone");
      ret.setAttribute("numM", numM + 1);
      if (varIntList.numberValues() > 0) ret.appendChild(varIntList.toXMLElement(doc));
      return ret;
    }

    //! Set the list of internal variables to be printed
    //! \param list list of indices of the internal variables
    void setVarIntList(const std::vector<size_t>& list);

    size_t numM;

   private:
    List<size_t> varIntList{"VarIntList"};
  };

  template <typename TDoc>
  typename TDoc::Node toXMLElement(TDoc& doc) const {
    auto ret = doc.createElement("Output");
    if (vtkStressStrain) ret.appendChild(vtkStressStrain.value().toXMLElement(doc));
    if (vtkFluxDGradD) ret.appendChild(vtkFluxDGradD.value().toXMLElement(doc));
    for (const auto& zone : zones) ret.appendChild(zone.toXMLElement(doc));
    for (const auto& ivarList : intVarList) ret.appendChild(ivarList.toXMLElement(doc));
    return ret;
  }

  //! Add zones of a material to .zstd output
  void addZone(const Zone& zone) { zones.push_back(zone); }
  void addZone(Zone&& zone) { zones.push_back(std::move(zone)); };
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
    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      auto ret = List<size_t>::toXMLElement(doc);
      ret.setAttribute("numM", numM + 1);
      return ret;
    }
  };

  std::vector<Zone> zones;
  std::vector<VtkIntVarList> intVarList;
};

}  // namespace amitex

#endif  // _AMITEX_OUTPUT_HEADER_