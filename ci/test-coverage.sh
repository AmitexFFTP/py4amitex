#/bin/bash

set -e

if [[ "$1" ]]
then
    SUMMARY="$1"
else
    SUMMARY="coverage-summary.txt"
fi

ROOT_DIR="$PWD"
BUILD_DIR="${ROOT_DIR}/build-cov"
INSTALL_DIR="${ROOT_DIR}/_install-cov"

cmake -S. -B${BUILD_DIR}  -DCOVERAGE=ON -DAMITEX_PYTHON=ON -Dpybind11_DIR=$(python -m pybind11 --cmakedir) -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
cmake --build ${BUILD_DIR} -j 4
cmake --install ${BUILD_DIR}
cd "${BUILD_DIR}"
ctest -T Test -T Coverage
cd "${ROOT_DIR}"
# ${INSTALL_DIR}/lib
echo "---------- Python -----" > "${SUMMARY}"
 PYTHONPATH=${INSTALL_DIR}/lib pytest --cov=${INSTALL_DIR}/lib --cov-report=xml:coverage-python.xml \
    --cov-report=html:coverage-python --cov-report=markdown:coverage-python.md --cov-report=term-missing \
    --cov-fail-under=0
head -n 1 coverage-python.md >> "${SUMMARY}"
tail -n 1 coverage-python.md >> "${SUMMARY}"

echo "---------- C++ -----" >> "${SUMMARY}"
mkdir -p coverage-cpp
gcovr --object-dir=${BUILD_DIR} -f "${ROOT_DIR}/cpp4amitex/src/" -f "${ROOT_DIR}/cpp4amitex/include/" \
    -f "${ROOT_DIR}/cpp4amitex/bindings/python/src/" \
    --print-summary --xml coverage-cpp.xml --html-details -o coverage-cpp/coverage.html \
    --fail-under-line 75 >> "${SUMMARY}"


cd "${ROOT_DIR}"