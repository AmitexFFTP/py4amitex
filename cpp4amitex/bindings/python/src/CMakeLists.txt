set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(input MODULE bindings.cpp input_general.cpp simulation.cpp field.cpp material_builder.cpp)

target_link_libraries(input PRIVATE AmitexCpp)

install(TARGETS input LIBRARY DESTINATION ${PYTHON_DEST}/py4amitex)

if(AMITEX_TESTS)
	pybind11_add_module(testutils MODULE bindings_testutils.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../../../tests/testutils.cpp)
	target_include_directories(testutils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../tests)
	target_link_libraries(testutils PRIVATE AmitexCpp)
	install(TARGETS testutils LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../tests)
endif()
