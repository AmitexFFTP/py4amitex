include_directories(.)

option(MEROPE_EXAMPLES "Compile Mérope examples" OFF)

if(MEROPE_EXAMPLES)
    add_subdirectory(merope)
endif()

add_library(pmvox pmvox.cpp)
target_link_libraries(pmvox AmitexCpp)
target_compile_definitions(pmvox PUBLIC ${CMAKE_CXX_BYTE_ORDER})

if(AMITEX_API)
    add_subdirectory(amitex_by_cmd_file)

    add_executable(supervise_mpi supervise_mpi.cpp )
    target_link_libraries(supervise_mpi AmitexCpp pmvox MPI::MPI_CXX)

    add_executable(supervise_simple supervise_simple.cpp)
    target_link_libraries(supervise_simple AmitexCpp pmvox MPI::MPI_CXX)
endif()

add_executable(simple_zones simple_zones.cpp)
target_link_libraries(simple_zones AmitexCpp pmvox)

add_executable(one_sphere one_sphere.cpp)
target_link_libraries(one_sphere AmitexCpp)

add_executable(minimal minimal.cpp)
target_link_libraries(minimal AmitexCpp)

add_executable(composite_continu composite_continu.cpp)
target_link_libraries(composite_continu AmitexCpp)
