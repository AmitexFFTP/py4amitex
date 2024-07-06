\page GettingStarted Getting Started
[TOC]

# AmitexCpp

C++ interface for [AMITEX-FFTP](https://www.maisondelasimulation.fr/projects/amitex).

## Build

First fetch submodules dependencies, for example with
```
git submodule update --init
```

CMake. For example:
```sh
cmake . -Bbuild -DCMAKE_INSTALL_PREFIX=YOUR_INSTALL_DIR
cmake --build build
cmake --install build
```

## Usage

A minimalist example code calling defining and lauching a simulation is provided in `examples/minimal.cpp`.

Compiling `your_program` from `your_program.cpp` can be done with (assuming the library was compiled with the corresponding `mpic++`):
```sh
mpic++ -o your_program -IYOUR_INSTALL_DIR/include your_program.cpp -LYOUR_INSTALL_DIR/lib -lAmitexCpp
```

For running or driving AMITEX simulations, you need to have `amitex_fftp` in your path.

### With Cmake

In your `CMakeList.txt`, the library target `AmitexCpp::AmitexCpp` can be imported with `find_package`:
```cmake
find_package(AmitexCpp)

add_executable(your_program your_program.cpp)
target_link_libraries(your_program AmitexCpp::AmitexCpp)
```

To find the package config files, the variable `CMAKE_PREFIX_PATH` should contain the directory `YOUR_INSTALL_DIR/lib/cmake`.

### External driving

It needs an AMITEX installation with the driver library (`libamitex-shared.so`), this option can be activated
with `-DAMITEX_API=ON`.

```sh
cmake . -Bbuild -DAMITEX_API=ON
```

By default, the library is searched in `$AMITEX_PATH/libAmitex/lib`. This assumes that AMITEX environnment variables are defined
(usually done with `source AMITEX_ROOT_DIR/env_amitex.sh`).
Otherwise, you can pass the flag `-DAMITEX_ROOT=AMITEX_ROOT_DIR` to CMake.

## Tests

CTest. In a CMake `build` directory:
```
ctest --output-on-failure
```

## Dependencies

- (Build) cmake >= 3.22
- (Build) pybind11 (submodule)
- (Tests & Python bindings) python >= 3.8

## Python bindings

For general usage, at the root of this repository,
```
pip install .
```

See `bindings/python/examples/minimal.py` for a minimal example

### Developping and testing

Activate python bindings with CMake option `AMITEX_PYTHON` set to `ON`. After building and installing, python modules are in `YOUR_INSTALL_DIR/lib`.

Tests can be launched with
```
PYTHONPATH=<YOUR_INSTALL_DIR> pytest
```

## Documentation

Provided that Doxygen was found on your system, docs can be build with the `docs` CMake target
```
cmake --build build -t docs
```
or, by changing into the `docs` directory
```
doxygen
```

Docs can then be browsed at `docs/html/index.html`.

With a few exceptions, only the C++ API is documented, as the python API is very similar.

## Compilers and compatibility

Only tested on Linux. Tested mainly with gcc 11.4.0 (Ubuntu 22.04)

Known to work with:
- gcc 11.4.0 (Ubuntu 22.04)
- clang 14.0.0 (Ubuntu 22.04)
- gcc 8.1.0 (CentOS 7)
- icpx/icpc in oneAPI 2022.2.1 (Ubuntu 22.04)
    (**note**: version 2021.1 compiles the c++ lit but chokes on the python bindings)

Known not to work with:
- compilers not supporting c++17
- compiler/system where the compiler use an old system standard library not supportting c++17

Some stdc++ versions need to link with `-lstdc++fs` in addition (the current build system adds it for gcc < 9)

**Note**

The standard C++ library (libstdc++) on some old compiler/systems need to use 
