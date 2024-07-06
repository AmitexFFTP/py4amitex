include_directories(.)

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

set(MEROPE_LIBRARIES GeomVER Grid VTKinout VoroInterface GeneOrientations OptiLaguerre RSAalgolib Physics FFTGrid)

add_library(amitex_merope amitex_compute.cpp)
target_compile_definitions(amitex_merope PUBLIC ${CMAKE_CXX_BYTE_ORDER})
target_include_directories(amitex_merope PUBLIC ${MEROPE_INCLUDE})
target_link_directories(amitex_merope PUBLIC ${MEROPE_LIB})
target_link_libraries(amitex_merope AmitexCpp ${MEROPE_LIBRARIES} ${OpenMP_CXX_LIBRARIES} fftw3)

add_executable(simple_spheres simple_spheres.cpp)
target_include_directories(simple_spheres PUBLIC ${MEROPE_INCLUDE})
target_link_directories(simple_spheres PUBLIC ${MEROPE_LIB})
target_link_libraries(simple_spheres amitex_merope)

add_executable(clay_with_lead_spheres clay_with_lead_spheres.cpp)
target_include_directories(clay_with_lead_spheres PUBLIC ${MEROPE_INCLUDE})
target_link_directories(clay_with_lead_spheres PUBLIC ${MEROPE_LIB})
target_link_libraries(clay_with_lead_spheres amitex_merope)

add_executable(simple_spheres_composite simple_spheres_composite.cpp)
target_include_directories(simple_spheres_composite PUBLIC ${MEROPE_INCLUDE})
target_link_directories(simple_spheres_composite PUBLIC ${MEROPE_LIB})
target_link_libraries(simple_spheres_composite amitex_merope)

add_library(amitex_make_input amitex_make_input.cpp)
target_include_directories(amitex_make_input PUBLIC ${MEROPE_INCLUDE})
target_link_directories(amitex_make_input PUBLIC ${MEROPE_LIB})
target_link_libraries(amitex_make_input AmitexCpp ${MEROPE_LIBRARIES} ${OpenMP_CXX_LIBRARIES} fftw3)

add_executable(simple_sphere_with_build_mat simple_sphere_with_build_mat.cpp)
target_link_libraries(simple_sphere_with_build_mat AmitexCpp amitex_make_input)

add_executable(simple_sphere_with_build_mat_composite simple_sphere_with_build_mat_composite.cpp)
target_link_libraries(simple_sphere_with_build_mat_composite AmitexCpp amitex_make_input)
