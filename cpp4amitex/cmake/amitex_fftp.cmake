
include(ExternalProject)

# to build libAmitex.so from .a
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_Fortran_PREPROCESS ON)

if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
	set(Fortran_NOUS_FLAG -fno-underscoring)
	set(Fortran_DEFINT_FLAG -fdefault-integer-8)
elseif(CMAKE_Fortran_COMPILER_ID STREQUAL INTEL)
	set(Fortran_NOUS_FLAG -assume nounderscore)
	set(Fortran_DEFINT_FLAG -i8)
else()
	message(FATAL "Unsupported Fortran compiler: ${CMAKE_Fortran_COMPILER_ID}")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
	set(Fortran_FAST_FLAG -ffast-math)
else()
	set(Fortran_FAST_FLAG "")
endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
	set(CMAKE_Fortran_FLAGS "-fcheck=bounds ${CMAKE_Fortran_FLAGS}")
endif()
endif()

set(Fortran_UMAT_FLAGS ${Fortran_FAST_FLAG} ${Fortran_NOUS_FLAG} ${Fortran_DEFINT_FLAG})

if(FFTW3_ROOT)
	set(FFTW3_INCLUDE "${FFTW3_ROOT}/include")
else()
	set(FFTW3_INCLUDE "/usr/include")
endif()

include_directories(${FFTW3_INCLUDE} ${MPI_Fortran_INCLUDE_DIRS})

if(NOT AMITEX_SOURCE_DIR)
set(AMITEX_SOURCE_DIR ${PROJECT_SOURCE_DIR}/../amitex_fftp)
endif()

set(FoX_SOURCE_DIR "${AMITEX_SOURCE_DIR}/lib_extern/FoX-4.1.2_modif2018")
ExternalProject_Add(Fox 
	SOURCE_DIR ${FoX_SOURCE_DIR}
	CMAKE_ARGS -DFoX_ENABLE_EXAMPLES=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
	INSTALL_COMMAND "")

set(FoX_BUILD_DIR "${PROJECT_BINARY_DIR}/Fox-prefix/src/Fox-build")

set(DECOMP_SOURCE_DIR "${AMITEX_SOURCE_DIR}/lib_extern/2decomp_fft")
ExternalProject_Add(decomp 
	SOURCE_DIR ${DECOMP_SOURCE_DIR}
	CMAKE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
	INSTALL_COMMAND "")

set(DECOMP_BUILD_DIR "${PROJECT_BINARY_DIR}/decomp-prefix/src/decomp-build")

set(AMITEX_SRC ${AMITEX_SOURCE_DIR}/libAmitex/src)

set(UMAT_AMITEX_SOURCES
	${AMITEX_SRC}/materiaux/contrainte_imposee.f90
	${AMITEX_SRC}/materiaux/elasaniso.f90
	${AMITEX_SRC}/materiaux/elasiso_eigs.f90
	${AMITEX_SRC}/materiaux/elasiso_GD.f90
	${AMITEX_SRC}/materiaux/thermoelasiso.f90
	${AMITEX_SRC}/materiaux/elasaniso_eigs.f90
	${AMITEX_SRC}/materiaux/elasaniso_GD.f90
	${AMITEX_SRC}/materiaux/elasiso.f90
	${AMITEX_SRC}/materiaux/paramextelasiso.f90
	${AMITEX_SRC}/materiaux/viscoelas_maxwell.f90
)

add_library(UmatAmitex SHARED ${UMAT_AMITEX_SOURCES})
target_compile_options(UmatAmitex PRIVATE ${Fortran_UMAT_FLAGS})

set(UMAT_AMITEXK_SOURCES
	${AMITEX_SRC}/materiauxK/Fourier_iso.f90
	${AMITEX_SRC}/materiauxK/Fourier_iso_polarization.f90 
	${AMITEX_SRC}/materiauxK/Homogeneous_Flux.f90
)

add_library(UmatAmitexK SHARED ${UMAT_AMITEXK_SOURCES})
target_compile_options(UmatAmitexK PRIVATE ${Fortran_UMAT_FLAGS})


set(AMITEX_user_functions
	${AMITEX_SRC}/user_functions_mod.f90
)

add_library(user_functions SHARED ${AMITEX_user_functions})

set(AMITEX_MOD
	${AMITEX_SRC}/simu_mod.f90
	${AMITEX_SRC}/amitex_mod.f90
	${AMITEX_SRC}/resolution_mod.f90
	${AMITEX_SRC}/NL_base_mod.f90
	${AMITEX_SRC}/sortie_std_mod.f90
	${AMITEX_SRC}/error_mod.f90
	${AMITEX_SRC}/material_mod.f90
	${AMITEX_SRC}/param_algo_mod.f90
	${AMITEX_SRC}/loading_mod.f90
	${AMITEX_SRC}/io_amitex_mod.f90
	${AMITEX_SRC}/io2_amitex_mod.f90
	${AMITEX_SRC}/green_mod.f90
	${AMITEX_SRC}/algo_functions_mod.f90
	${AMITEX_SRC}/linear_mod.f90
	${AMITEX_SRC}/read_geom.f90
	${AMITEX_SRC}/field_mod.f90
	${AMITEX_SRC}/non_local_mod.f90
)

set(AMITEX_user
	${AMITEX_SRC}/amitex_user_mod.f90
	${AMITEX_SRC}/resolution_user_mod.f90
	${AMITEX_SRC}/standard_user_mod.f90
	${AMITEX_SRC}/non_local_user_mod.f90)

set(UMAT_C
	${AMITEX_SRC}/umat_load.c
	${AMITEX_SRC}/umat_call.c
	${AMITEX_SRC}/umatD_call.c)

set(AMITEX_MAIN ${AMITEX_SRC}/amitex_fftp.f90)

set(AMITEX_LIB_SRC
	${AMITEX_SRC}/simu_mod.f90
	${AMITEX_SRC}/amitex_mod.f90
	${AMITEX_SRC}/resolution_mod.f90
	${AMITEX_SRC}/NL_base_mod.f90
	${AMITEX_SRC}/sortie_std_mod.f90
	${AMITEX_SRC}/error_mod.f90
	${AMITEX_SRC}/material_mod.f90
	${AMITEX_SRC}/param_algo_mod.f90
	${AMITEX_SRC}/loading_mod.f90
	${AMITEX_SRC}/io2_amitex_mod.f90
	${AMITEX_SRC}/green_mod.f90
	${AMITEX_SRC}/algo_functions_mod.f90
	${AMITEX_SRC}/linear_mod.f90
	${AMITEX_SRC}/read_geom.f90
	${AMITEX_SRC}/field_mod.f90
	${AMITEX_SRC}/non_local_mod.f90
	${AMITEX_SRC}/io_amitex_mod.f90
	${AMITEX_SRC}/amitex_mpi.f90
	${AMITEX_SRC}/amitex_capi.f90
)

add_library(Amitex SHARED ${AMITEX_LIB_SRC} ${AMITEX_user} ${UMAT_C})
target_include_directories(Amitex PUBLIC "${PROJECT_BINARY_DIR}/lib/modules" "${FoX_BUILD_DIR}/modules" "${DECOMP_BUILD_DIR}/include")
target_compile_definitions(Amitex PUBLIC NEW_2DECOMP)
target_link_directories(Amitex PUBLIC "${FoX_BUILD_DIR}/lib" "${DECOMP_BUILD_DIR}/lib")
set_property(TARGET Amitex PROPERTY Fortran_MODULE_DIRECTORY ${PROJECT_BINARY_DIR}/lib/modules)

target_link_libraries(Amitex user_functions  decomp2d fftw3 FoX_dom FoX_sax FoX_fsys FoX_utils FoX_common ${MPI_Fortran_LIBRARIES})

add_executable(amitex_fftp ${AMITEX_MAIN})
target_link_libraries(amitex_fftp Amitex)

install(TARGETS amitex_fftp user_functions Amitex
        RUNTIME DESTINATION libAmitex/bin
        LIBRARY DESTINATION libAmitex/lib
		ARCHIVE DESTINATION libAmitex/lib)

install(TARGETS UmatAmitex DESTINATION libAmitex/src/materiaux)
install(TARGETS UmatAmitexK DESTINATION libAmitex/src/materiauxK)
