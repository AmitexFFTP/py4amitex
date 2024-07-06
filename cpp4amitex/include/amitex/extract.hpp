#ifndef _AMITEX_EXTRACT_HEADER_
#define _AMITEX_EXTRACT_HEADER_

#include <array>
#include <filesystem>
#include <string_view>
#include <vector>

#include "amitex/errors.hpp"
#include "amitex/input/common.hpp"

namespace amitex {

class Extract {
 public:
  Extract() {}
  Extract(std::string_view prefix) : prefix{prefix} {}
  //! \return the last computed strain (indexing: XX YY ZZ XY XZ YZ YZ ZX ZY)
  Tensor3D averageStrain();
  //! \return the last computed stress (indexing: XX YY ZZ XY XZ YZ YZ ZX ZY)
  Tensor3D averageStress();
  //! \return the last computed diffusion gradient
  Vector3D averageDiffusionGradient(int indexD);
  //! \return the last computed diffusion flux
  Vector3D averageDiffusionFlux(int indexD);

 private:
  using TensorLin = std::array<double, 9>;
  struct MechanicalQuantities {
    double t;  //!< time
    TensorLin stress, strain;
    TensorLin sigStress, sigStrain;  //!< standard deviations
    int niter;                       //!< number of iterations
  };
  struct DiffusionQuantities {
    double t;  //!< time
    Vector3D gradient, flux;
    Vector3D sigGradient, sigFlux;  //!< standard deviations
    int niter;                      //!< number of iterations
  };

  std::string prefix = "output";
  DiffusionQuantities currentDiffusion;
  MechanicalQuantities currentMechanics;
  std::filesystem::file_time_type lastWrite;

  void setSymComponents(TensorLin& values);
  void readStd(DiffusionQuantities& q);
  void readStd(MechanicalQuantities& q);
  static Tensor3D toTensor3D(const TensorLin& tensor);
};

}  // namespace amitex
#endif  // _AMITEX_EXTRACT_HEADER_