#include "amitex/simulation.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "amitex/errors.hpp"

namespace amitex {

static std::string getLastLine(const std::string& path);

std::string getSimulationShellCommand(const Input& input, int numberProcs) {
  std::string cmd = "amitex_fftp";
  std::vector<std::string> args = {"-nm", input.materialIdsPath(), "-nz", input.zoneIdsPath(),
                                   "-a",  input.algorithmPath(),   "-m",  input.materialsPath(),
                                   "-c",  input.loadingPath(),     "-s",  input.outputPrefix()};
  for (const auto& arg : args) cmd += ' ' + arg;
  if (numberProcs == 0) {
    cmd = "mpirun " + cmd;
  } else {
    cmd = "mpirun -n " + std::to_string(numberProcs) + " " + cmd;
  }
  return cmd;
}

void runSimulationExternal(Input& input, int numberProcs) {
  if (numberProcs < 0) throw AmitexError{"number of processes must be >= 0", 1};
  input.generateFiles();

  std::string cmd = getSimulationShellCommand(input, numberProcs);
  std::cerr << "LAUNCH AMITEX: " << cmd << std::endl;
  int exc = std::system(cmd.c_str());
  if (exc != 0) throw AmitexError{"simulation failed, see " + input.outputPrefix() + ".log", exc};
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
