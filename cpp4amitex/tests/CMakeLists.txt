include_directories(.)

set(AMITEX_TESTS_SRC
	testutils.cpp
	mat2zone2_prepare.cpp
	gen_xml.cpp
	input_loadingalgo.cpp
	add_zone_launch.cpp
	bilayer.cpp
	material_general.cpp
	composite_prism.cpp
	elasiso_eigs.cpp
	composite_interphase.cpp
	grid_common.cpp
	tfields.cpp
	composite_builder.cpp
	tio.cpp
	tmaterial_builder.cpp
)

if(AMITEX_API)
	set(AMITEX_TESTS_SRC
		${AMITEX_TESTS_SRC}
		add_zone_run.cpp
	)
endif()


add_executable(amitex_tests 
	${AMITEX_TESTS_SRC}
)
target_link_libraries(amitex_tests AmitexCpp GTest::gtest_main pthread)

include(GoogleTest)
gtest_discover_tests(amitex_tests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} XML_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/gtest_xml)
