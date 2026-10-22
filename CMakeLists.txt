# SPDX-License-Identifier: LGPL-3.0-only
# (C) copyright 2026 of CEA
# author: Guillaume Bareigts <guillaume.bareigts@cea.fr>, CEA

cmake_minimum_required(VERSION 3.22)

project(py4amitex LANGUAGES CXX VERSION 1.1.0)

include(CTest)

if(DEFINED SKBUILD_PROJECT_NAME)
	set(PYTHON_DEST .)
else()
	set(PYTHON_DEST lib)
endif()


add_subdirectory(cpp4amitex)

install(DIRECTORY py4amitex DESTINATION ${PYTHON_DEST}
	PATTERN "tests" EXCLUDE
	PATTERN "__pycache__" EXCLUDE
)
