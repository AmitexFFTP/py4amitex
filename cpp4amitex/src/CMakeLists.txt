include_directories(.)

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
	xml_utils.cpp
)

if(AMITEX_API)
set(SOURCES_DRIVING
	component.cpp
	simulation.cpp
)
else()
set(SOURCES_DRIVING)
endif()

add_library(AmitexCpp ${SOURCES_INPUT} ${SOURCES_DRIVING})
target_compile_definitions(AmitexCpp PUBLIC "AMITEX_${CMAKE_CXX_BYTE_ORDER}")
if(AMITEX_API)
	target_link_libraries(AmitexCpp ${AMITEX_LIBRARIES} MPI::MPI_CXX)
endif()
# For python
set_property(TARGET AmitexCpp PROPERTY POSITION_INDEPENDENT_CODE ON)

install(TARGETS AmitexCpp EXPORT AmitexCppTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		INCLUDES DESTINATION include
	)
