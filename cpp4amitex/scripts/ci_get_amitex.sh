#/bin/bash

set -e

COMMIT=2e655044b50d7f061540617049113b503a68e3d8
ACCESS_TOKEN=$1
mkdir -p build
cd build
if [[ ! -d  amitex_fftp ]]
then
	git clone https://oauth2:$ACCESS_TOKEN@gitlab.maisondelasimulation.fr/amitex/amitex_fftp.git
fi
cd amitex_fftp
git fetch
git checkout $COMMIT
cp install install_gnu
sed -i "/^FC\=/c\FC=gfortran" install_gnu
sed -i "/^MPIFC\=/c\MPIFC=mpif90" install_gnu
bash install_gnu
cd ..
