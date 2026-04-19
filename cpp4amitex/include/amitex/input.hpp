#ifndef _AMITEX_INPUT_HEADER_
#define _AMITEX_INPUT_HEADER_

#include <filesystem>
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

// Type of VTK generation for Materials/Zones
enum class VtkGeneration {
  Generic,          // Ids VTK are generated
  OneMaterial,      // only Zones VTK is generated
  OneZone,          // only Zones VTK is generated
  OneZonePerVoxel,  // No VTK is generated
};

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
  //! Path to VTK file defining material placement (empty if not generated)
  std::string materialIdsPath() const;
  //! Path to VTK file defining zones  (empty if not generated)
  std::string zoneIdsPath() const;
  //! Prefix to output files
  std::string outputPrefix() const { return resultsDir + "/output/output"; }

  //! Generate Algorithm Parameters XML file
  void generateAlgorithm(const std::filesystem::path& path);
  //! Generate Algorithm Parameters XML file
  //! \param path
  //! \param coeffDirectory directory where coefficients/intvars/composite data will be stored
  void generateMaterials(const std::filesystem::path& path,
                         const std::filesystem::path& coeffDirectory);
  //! Generate Loading&Output XML file
  void generateLoadingOutput(const std::filesystem::path& path);
  //! Generate Material IDs VTK file
  void generateMaterialVTK(const std::filesystem::path& path);
  //! Generate Zone IDs VTK file
  void generateZoneVTK(const std::filesystem::path& path);
  //! Generate AMITEX 'commands' file
  //! \param path path to generated file
  //! \param algorithmPath path to the XML file containing algorithm parameters
  //! \param materialsPath path to the XML file containing materials
  //! \param loadingPath path to the XML file containing loading and output
  //! \param materialIdsPath path to the VTK file containing material IDs
  //! \param zoneIdsPath path to the VTK file containing zone IDs
  //! \param outputPrefix prefix of AMITEX output paths
  void generateCommandFile(const std::filesystem::path& path,
                           const std::filesystem::path& algorithmPath,
                           const std::filesystem::path& materialsPath,
                           const std::filesystem::path& loadingPath,
                           const std::filesystem::path& materialIdsPath,
                           const std::filesystem::path& zoneIdsPath,
                           const std::filesystem::path& outputPrefix);

  //! Determine types of Ids VTKs to generate
  VtkGeneration getVtkGeneration() const;

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
