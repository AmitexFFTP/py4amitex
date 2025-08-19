#ifndef _AMITEX_SIMULATION_HEADER_
#define _AMITEX_SIMULATION_HEADER_

#include "amitex/config.hpp"

#ifdef AMITEX_DRIVER_API
#include <mpi.h>
#endif

#include "amitex/input.hpp"

//! \file simulation.hpp
//! \brief run AMITEX-FFTP simulations from \ref amitex::Input

namespace amitex {

//! \brief Initialize input files and launch simulation by executing `amitex_fftp`
//! \param input input used by the simulation
//! \param numberProcs number of MPI processes used
//! \throw AmitexError when the simulation does no exit normally
//!
//! When `numberProcs` is 0, the number of available MPI processes is used (exact behavior depends
//! on the MPI environment)
void runSimulationExternal(Input& input, int numberProcs = 0);

//! \return shell command to run `amitex_fftp` (with mpirun)
//! \param input input used by the simulation
//! \param numberProcs number of MPI processes used
std::string getSimulationShellCommand(const Input& input, int numberProcs = 0);

//! \brief shell command to run `amitex_fftp` (with mpirun)
//! \param algorithmPath path to the XML file containing algorithm parameters
//! \param materialsPath path to the XML file containing materials
//! \param loadingPath path to the XML file containing loading and output
//! \param materialIdsPath path to the VTK file containing material IDs
//! \param zoneIdsPath path to the VTK file containing zone IDs
//! \param outputPrefix prefix of AMITEX output paths
//! \param numberProcs requested number of MPI processes (omited if equal to 0)
std::string getSimulationShellCommandFromFiles(const std::filesystem::path& algorithmPath,
                                               const std::filesystem::path& materialsPath,
                                               const std::filesystem::path& loadingPath,
                                               const std::filesystem::path& materialIdsPath,
                                               const std::filesystem::path& zoneIdsPath,
                                               const std::filesystem::path& outputPrefix,
                                               int numberProcs = 0);

//! @brief simulation by executing `amitex_fftp` with the files as input
//! \param algorithmPath path to the XML file containing algorithm parameters
//! \param materialsPath path to the XML file containing materials
//! \param loadingPath path to the XML file containing loading and output
//! \param materialIdsPath path to the VTK file containing material IDs
//! \param zoneIdsPath path to the VTK file containing zone IDs
//! \param outputPrefix prefix of AMITEX output paths
//! \param numberProcs requested number of MPI processes (omited if equal to 0)
void runSimulationFromFiles(const std::filesystem::path& algorithmPath,
                            const std::filesystem::path& materialsPath,
                            const std::filesystem::path& loadingPath,
                            const std::filesystem::path& materialIdsPath,
                            const std::filesystem::path& zoneIdsPath,
                            const std::filesystem::path& outputPrefix, int numberProcs = 0);

#ifdef AMITEX_DRIVER_API
//! Run a simulation with the amitex library
//! \param input Input
//! \param comm MPI communicator
void runSimulation(Input& input, MPI_Comm comm = MPI_COMM_WORLD);
#endif

}  // namespace amitex

#endif  // _AMITEX_SIMULATION_HEADER_
