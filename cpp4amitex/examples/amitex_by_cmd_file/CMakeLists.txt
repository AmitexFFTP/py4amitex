
add_executable(supervise_by_cmd supervise_by_cmd.cpp )
target_link_libraries(supervise_by_cmd AmitexCpp MPI::MPI_CXX)

add_executable(solve_by_cmd solve_by_cmd.c )
target_link_libraries(solve_by_cmd AmitexCpp MPI::MPI_CXX)
