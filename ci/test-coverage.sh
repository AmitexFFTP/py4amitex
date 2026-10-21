#/bin/bash

set -e

ROOT_DIR="$PWD"
BUILD_DIR="${ROOT_DIR}/build-cov"
INSTALL_DIR="${ROOT_DIR}/_install-cov"

cmake -S. -B${BUILD_DIR}  -DCOVERAGE=ON -DAMITEX_PYTHON=ON -Dpybind11_DIR=$(python -m pybind11 --cmakedir) -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
cmake --build ${BUILD_DIR} -j 4
cd "${BUILD_DIR}"
ctest -T Test -T Coverage
cd "${ROOT_DIR}"
cmake --install ${BUILD_DIR}
# ${INSTALL_DIR}/lib
PYTHONPATH=${INSTALL_DIR}/lib pytest --cov=${INSTALL_DIR}/lib --cov-report=xml --cov-report=html --cov-report=term-missing --cov-fail-under=0
cd "${BUILD_DIR}"
gcovr -r ..  -f "${ROOT_DIR}/cpp4amitex/src/" -f "${ROOT_DIR}/cpp4amitex/include/" -f "${ROOT_DIR}/cpp4amitex/bindings/python/src/" \
    --print-summary --xml ../coverage-cpp.xml --html-details -o coverage-cpp
cd "${ROOT_DIR}"