#/bin/bash

set -e

cmake -S. -B_build-cov  -DCOVERAGE=ON -DAMITEX_PYTHON=OFF
cd _build-cov
cmake --build . -j 2
ctest -T Test -T Coverage
gcovr -r .. --html-details -o coverage.html --print-summary  \
	--exclude-directories=_build-cov/_deps --exclude-directories=tests --exclude-directories=bindings --exclude-directories=examples 
cd ..
