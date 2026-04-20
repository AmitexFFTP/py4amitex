#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "amitex/errors.hpp"

namespace amitex {

static std::string getLastLine(const std::string& path);

std::string getSimulationShellCommandFromFiles(const std::filesystem::path& algorithmPath,
                                               const std::filesystem::path& materialsPath,
                                               const std::filesystem::path& loadingPath,
                                               const std::filesystem::path& materialIdsPath,
                                               const std::filesystem::path& zoneIdsPath,
                                               const std::filesystem::path& outputPrefix,
                                               const Grid& grid, int numberProcs) {
  std::string cmd = "amitex_fftp";
  std::vector<std::string> args;
  if (!materialIdsPath.empty()) {
    args.push_back("-nm");
    args.push_back(materialIdsPath);
  }
  if (!zoneIdsPath.empty()) {
    args.push_back("-nz");
    args.push_back(zoneIdsPath);
  }
  args.push_back("-a");
  args.push_back(algorithmPath);
  args.push_back("-m");
  args.push_back(materialsPath);
  args.push_back("-c");
  args.push_back(loadingPath);
  args.push_back("-s");
  args.push_back(outputPrefix);

  if (materialIdsPath.empty() && zoneIdsPath.empty()) {
    args.push_back("-NX");
    args.push_back(std::to_string(grid.dims()[0]));
    args.push_back("-NY");
    args.push_back(std::to_string(grid.dims()[1]));
    args.push_back("-NZ");
    args.push_back(std::to_string(grid.dims()[2]));
    args.push_back("-DX");
    args.push_back(std::to_string(grid.voxelLengths()[0]));
    args.push_back("-DY");
    args.push_back(std::to_string(grid.voxelLengths()[1]));
    args.push_back("-DZ");
    args.push_back(std::to_string(grid.voxelLengths()[2]));
  }

  for (const auto& arg : args) cmd += ' ' + arg;
  if (numberProcs == 0) {
    cmd = "mpirun " + cmd;
  } else {
    cmd = "mpirun -n " + std::to_string(numberProcs) + " " + cmd;
  }
  return cmd;
}

std::string getSimulationShellCommand(const Input& input, int numberProcs) {
  return getSimulationShellCommandFromFiles(
      input.algorithmPath(), input.materialsPath(), input.loadingPath(), input.materialIdsPath(),
      input.zoneIdsPath(), input.outputPrefix(), input.grid, numberProcs);
}

void runSimulationFromFiles(const std::filesystem::path& algorithmPath,
                            const std::filesystem::path& materialsPath,
                            const std::filesystem::path& loadingPath,
                            const std::filesystem::path& materialIdsPath,
                            const std::filesystem::path& zoneIdsPath,
                            const std::filesystem::path& outputPrefix, const Grid& grid,
                            int numberProcs) {
  if (numberProcs < 0) throw AmitexError{"number of processes must be >= 0", 1};
  std::string cmd =
      getSimulationShellCommandFromFiles(algorithmPath, materialsPath, loadingPath, materialIdsPath,
                                         zoneIdsPath, outputPrefix, grid, numberProcs);
  std::cerr << "LAUNCH AMITEX: " << cmd << std::endl;
  int exc = std::system(cmd.c_str());
  if (exc != 0) throw AmitexError{"simulation failed, see " + outputPrefix.string() + ".log", exc};
}

void runSimulationExternal(Input& input, int numberProcs) {
  input.generateFiles();
  runSimulationFromFiles(input.algorithmPath(), input.materialsPath(), input.loadingPath(),
                         input.materialIdsPath(), input.zoneIdsPath(), input.outputPrefix(),
                         input.grid, numberProcs);
}

#if 0
void runSimulationExternalMPI(Input& input, int numberProcs, MPI_Comm comm) {
  if (numberProcs < 1) throw AmitexError{"number of processes must be > 0", 1};
  input.generateFiles();

  std::string cmd = "amitex_fftp";
  std::vector<std::string> args = {"-nm", input.materialIdsPath(), "-nz", input.zoneIdsPath(),
                                   "-a",  input.algorithmPath(),   "-m",  input.materialsPath(),
                                   "-c",  input.loadingPath(),     "-s",  input.outputPrefix()};
  int exc;
  int mpiInited;
  MPI_Initialized(&mpiInited);
  // We cannot launch mpirun in MPI
  if (!mpiInited) {
    cmd = "mpirun -n " + std::to_string(numberProcs) + " " + cmd;
    for (const auto& arg : args) cmd += ' ' + arg;
    std::cerr << "LAUNCH AMITEX: " << cmd << std::endl;
    exc = std::system(cmd.c_str());
  } else {
    MPI_Comm intercomm;
    std::vector<int> errcodes(numberProcs);
    std::fill(errcodes.begin(), errcodes.end(), 0);
    std::vector<char*> argv;
    for (auto& arg : args) argv.push_back(arg.data());
    argv.push_back(nullptr);
    for (auto arg : argv) {
      std::cout << arg << '\n';
    }
    exc = MPI_Comm_spawn(cmd.c_str(), argv.data(), numberProcs, MPI_INFO_NULL, 0, comm, &intercomm,
                         errcodes.data());
    if (exc == MPI_SUCCESS) {
      // MPI_Barrier(intercomm);
      // Nope, you would need to change AMITEX to be aware of the intercomm (with
      // MPI_Comm_get_parent)
      do {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(500ms);
      } while (
          getLastLine(input.outputPrefix() + ".log").find("temps global (hors initialisations)") ==
          std::string::npos);
      if (any_of(errcodes.begin(), errcodes.end(), [](int c) { return c != 0; })) {
        exc = *max_element(errcodes.begin(), errcodes.end());
      } else {
        exc = 0;
      }
    }
  }
  if (exc != 0) throw AmitexError{"simulation failed, see " + input.outputPrefix() + ".log", exc};
}

static std::string getLastLine(const std::string& path) {
  std::string line, ret;
  std::ifstream infs{path};
  if (infs.good()) {
    while (!std::getline(infs, line).eof()) {
      ret = line;
    }
  }
  return ret;
}

#endif

}  // namespace amitex
