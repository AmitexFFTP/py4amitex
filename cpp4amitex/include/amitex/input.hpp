#ifndef _AMITEX_INPUT_HEADER_
#define _AMITEX_INPUT_HEADER_

#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "amitex/input/algorithm_parameters.hpp"
#include "amitex/input/grid.hpp"
#include "amitex/input/loading.hpp"
#include "amitex/input/loading_output.hpp"
#include "amitex/input/materials.hpp"
#include "amitex/input/output.hpp"

#include "amitex/private/input_element.hpp"

namespace amitex {

//! Regroup all AMITEX input parameters
class Input {
 public:
  Grid grid;  //!< Grid defining the unit cell and number of voxels
  Ptr<AlgorithmParameters> algorithmParameters;  //!< see \ref AlgorithmParameters
  Ptr<Materials> materials;                      //!< see \ref Materials
  Ptr<LoadingOutput> loadingOutput;              //!< see \ref LoadingOutput
  std::string resultsDir = "amitex_results";     //!< Where generated and AMITEX output will be

  static Ptr<Input> create(const Grid& grid, Ptr<AlgorithmParameters> algorithmParameters,
                           Ptr<Materials> materials, Ptr<LoadingOutput> loadingOutput) {
    return makePtr<Input>(Input{grid, algorithmParameters, materials, loadingOutput});
  }
  //! Generate all input files (XML, VTK, BIN) in \ref resultsDir
  void generateFiles();

  //! Path to XML defining algorithm parameters
  std::string algorithmPath() const { return resultsDir + "/algorithm.xml"; }
  //! Path to XML file defining loading and output
  std::string loadingPath() const { return resultsDir + "/loading.xml"; }
  //! Path to XML file defining materials
  std::string materialsPath() const { return resultsDir + "/materials.xml"; }
  //! Path to VTK file defining material placement
  std::string materialIdsPath() const { return resultsDir + "/materialIds.vtk"; }
  //! Path to VTK file defining zones
  std::string zoneIdsPath() const { return resultsDir + "/zoneIds.vtk"; }
  //! Prefix to output files
  std::string outputPrefix() const { return resultsDir + "/output/output"; }

 private:
  Input(const Grid& grid, Ptr<AlgorithmParameters> algorithmParameters, Ptr<Materials> materials,
        Ptr<LoadingOutput> loadingOutput)
      : grid{grid},
        algorithmParameters{algorithmParameters},
        materials{materials},
        loadingOutput{loadingOutput} {}
};

}  // namespace amitex

#endif  // _AMITEX_INPUT_HEADER_
