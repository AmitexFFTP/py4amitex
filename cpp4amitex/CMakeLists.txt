

option(AMITEX_TESTS "Build tests" ON)

if(AMITEX_TESTS)
  # https://google.github.io/googletest/quickstart-cmake.html#set-up-a-project
  include(FetchContent)
  FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  # MakeAvailable will install gtest and we do not want that
  FetchContent_GetProperties(googletest)
  if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
endif()

option(COVERAGE "use code coverage" OFF)
if(COVERAGE)
	set(CMAKE_BUILD_TYPE "Debug")
	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} --coverage -O0")
	endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
add_compile_definitions("AMITEX_DEBUG")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0.0)
    message("gcc version ${CMAKE_CXX_COMPILER_VERSION}: link libstdc++fs")
    set(CMAKE_CXX_STANDARD_LIBRARIES -lstdc++fs ${CMAKE_CXX_STANDARD_LIBRARIES})
  endif()
endif()

option(AMITEX_API "Use AMITEX API" OFF)

if(AMITEX_API)
  find_package(MPI REQUIRED CXX)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
elseif(CMAKE_CXX_STANDARD LESS 17)
  message(FATAL_ERROR "C++ standard must be >= 17")
endif()

if(AMITEX_API)
    if(NOT DEFINED AMITEX_ROOT)
        set(AMITEX_ROOT $ENV{AMITEX_PATH})
    endif()
    message("AMITEX_ROOT = ${AMITEX_ROOT}")

    find_library(AMITEX_LIBRARY amitex-shared REQUIRED HINTS "${AMITEX_ROOT}/libAmitex/lib")
    find_library(AMITEX_USER_LIBRARY user_functions REQUIRED HINTS "${AMITEX_ROOT}/libAmitex/lib")
    set(AMITEX_LIBRARIES ${AMITEX_LIBRARY} ${AMITEX_USER_LIBRARY})
    find_file(AMITEX_HEADER "amitex.h" REQUIRED HINTS "${AMITEX_ROOT}/libAmitex/include")
    cmake_path(GET AMITEX_HEADER PARENT_PATH AMITEX_INCLUDE)
    include_directories(${AMITEX_INCLUDE})
    set(AMITEX_DRIVER_API ON) 
endif()

include_directories(include ${CMAKE_BINARY_DIR}/include)

add_subdirectory(src)
option(AMITEX_EXAMPLES "Build examples" ON)
if(AMITEX_EXAMPLES)
	add_subdirectory(examples)
endif()
if(AMITEX_TESTS)
	add_subdirectory(tests)
endif()

option(AMITEX_PYTHON "build AMITEX Python bindings" ON)
if(AMITEX_PYTHON)
	add_subdirectory(bindings/python/src)
endif()

if(NOT DEFINED SKBUILD_PROJECT_NAME)
  install(DIRECTORY include/amitex DESTINATION include)
  install(EXPORT AmitexCppTargets NAMESPACE AmitexCpp:: DESTINATION lib/cmake)

  include(CMakePackageConfigHelpers)
  configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/AmitexCppConfig.cmake"
    INSTALL_DESTINATION lib/cmake/
  )
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/AmitexCppConfigVersion.cmake"
    VERSION "${version}"
    COMPATIBILITY AnyNewerVersion
  )
  install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/AmitexCppConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/AmitexCppConfigVersion.cmake"
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/
  )
endif()

find_package(Doxygen)
if(DOXYGEN_FOUND)
  doxygen_add_docs(docs WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs)
endif()
