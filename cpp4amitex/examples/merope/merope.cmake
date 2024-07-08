if(NOT DEFINED MEROPE_INSTALL_DIR)
    set(MEROPE_INSTALL_DIR $ENV{MEROPE_PATH})
endif()
if(NOT DEFINED MEROPE_ROOT_DIR)
    set(MEROPE_ROOT_DIR $ENV{MEROPE_SOURCE_DIR})
endif()
message("Mérope: '${MEROPE_ROOT_DIR}'  '${MEROPE_INSTALL_DIR}'")

find_file(GRID_PATH Grid.hxx PATHS ${MEROPE_ROOT_DIR}/modules/merope_core/include/Grid REQUIRED)
find_library(GRID_LIBRARY Grid PATHS ${MEROPE_INSTALL_DIR}/lib REQUIRED)

set(MEROPE_INCLUDE 
    ${MEROPE_ROOT_DIR}/modules/merope_core/include
    ${MEROPE_ROOT_DIR}/modules/AlgoPacking/src
    ${MEROPE_ROOT_DIR}/modules/Eigen
    ${MEROPE_ROOT_DIR}/modules/voro-plus-plus/src
)
cmake_path(GET GRID_LIBRARY PARENT_PATH MEROPE_LIB)

find_package(OpenMP)

set(MEROPE_LIBRARIES GeomVER Grid VTKinout VoroInterface GeneOrientations OptiLaguerre RSAalgolib Physics FFTGrid ${OpenMP_CXX_LIBRARIES} fftw3)
