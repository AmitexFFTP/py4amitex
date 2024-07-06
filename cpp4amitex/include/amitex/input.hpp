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

class Input {
 public:
  Grid grid;                                  //!< Grid defining the unit cell and number of voxels
  AlgorithmParameters algorithmParameters;    //!< see \ref AlgorithmParameters
  Materials materials;                        //!< see \ref Materials
  LoadingOutput loadingOutput;                //!< see \ref LoadingOutput
  std::string resultsDir = "amitex_results";  //!< Where generated and AMITEX output will be

  Input() = default;
  Input(Grid&& grid) : grid{grid} {}
  Input(const Grid& grid) : grid{grid} {}
  Input(const Grid& grid, AlgorithmParameters&& algorithmParameters, Materials&& materials,
        LoadingOutput&& loadingOutput)
      : grid{grid},
        algorithmParameters{algorithmParameters},
        materials{materials},
        loadingOutput{loadingOutput} {}
  Input(const Grid& grid, const AlgorithmParameters& algorithmParameters,
        const Materials& materials, const LoadingOutput& loadingOutput)
      : grid{grid},
        algorithmParameters{algorithmParameters},
        materials{materials},
        loadingOutput{loadingOutput} {}

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
};

}  // namespace amitex

#endif  // _AMITEX_INPUT_HEADER_
