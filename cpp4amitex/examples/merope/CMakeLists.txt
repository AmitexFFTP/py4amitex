cmake_minimum_required(VERSION 3.22)

project(ex_chain_merope LANGUAGES CXX)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
elseif(CMAKE_CXX_STANDARD LESS 20)
  message(FATAL_ERROR "C++ standard must be >= 20")
endif()


find_package(AmitexCpp)

include(merope.cmake)

add_executable(simple_spheres_composite simple_spheres_composite.cpp)
target_link_libraries(simple_spheres_composite AmitexCpp::AmitexCpp ${MEROPE_LIBRARIES})
target_include_directories(simple_spheres_composite PUBLIC ${MEROPE_INCLUDE})
target_link_directories(simple_spheres_composite PUBLIC ${MEROPE_LIB})

add_executable(simple_spheres_diffusion simple_spheres_diffusion.cpp)
target_link_libraries(simple_spheres_diffusion AmitexCpp::AmitexCpp ${MEROPE_LIBRARIES})
target_include_directories(simple_spheres_diffusion PUBLIC ${MEROPE_INCLUDE})
target_link_directories(simple_spheres_diffusion PUBLIC ${MEROPE_LIB})

