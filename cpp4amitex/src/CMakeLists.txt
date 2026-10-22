# SPDX-License-Identifier: LGPL-3.0-only
# (C) copyright 2026 of CEA
# author: Guillaume Bareigts <guillaume.bareigts@cea.fr>, CEA

include_directories(.)

set(AMITEX_VERSION "${PROJECT_VERSION}")
configure_file(config.hpp.in ${CMAKE_BINARY_DIR}/include/amitex/config.hpp)
install(FILES ${CMAKE_BINARY_DIR}/include/amitex/config.hpp DESTINATION include/amitex)

set(SOURCES_INPUT
	input.cpp
	grid.cpp
	loading.cpp
	loading_output.cpp
	errors.cpp
	io.cpp
	extract.cpp
	zone.cpp
	basic_coeff.cpp
	material.cpp
	material_composite.cpp
	materials.cpp
	algorithm.cpp
	algorithm_parameters.cpp
	algorithm_composite.cpp
	intvar.cpp
	init_load_ext.cpp
	interphase.cpp
	output.cpp
	user_interruption.cpp
	material_builder.cpp
	run_external.cpp
	xml_document.cpp
)

add_library(AmitexCpp ${SOURCES_INPUT})
target_compile_definitions(AmitexCpp PUBLIC "AMITEX_${CMAKE_CXX_BYTE_ORDER}")

# For python
set_property(TARGET AmitexCpp PROPERTY POSITION_INDEPENDENT_CODE ON)

install(TARGETS AmitexCpp EXPORT AmitexCppTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		INCLUDES DESTINATION include
	)
