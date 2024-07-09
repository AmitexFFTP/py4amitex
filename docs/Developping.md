# Developping


## Build with CMake

With ``build`` and ``_install`` as build directory and install directory respectively:

    cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    cmake --install 

One can then add ``_install/lib`` to ``PYTHONPATH``.

## Tests

To launch python tests:

    pytest

To launch c++ tests:

    cmake --test-dir build --output-on-failure

## Documentation

It uses [Sphinx](https://www.sphinx-doc.org)

The python module(s) need to be compiled and in (python) path. In ``docs``

    make html

(use ``make`` to see available targets).

### C++ documentation

in ``cpp4amitex/docs``

    doxygen

### C++ to Doxygen documentation

Primary API documentation is is the C++, the wrapper uses the header ``cpp4amitex/bindings/python/src/docstrings.hpp``.
The file need be generated when the in source documentation is updated. In ``cpp4amitex`` run:

    python3 scripts/mkdocs_cpp_to_py.py

The script needs [pybind11_mkdocs](https://github.com/pybind/pybind11_mkdoc), which needs ``libclang``. If a suitable version of libclang
is not found, you need to set the environment variable ``LIBCLANG_PATH``.

