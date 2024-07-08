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

