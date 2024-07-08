#!/bin/sh

# config.hpp needs to be generated beforehand

# pip3 install pybind11_mkdoc
python3 -m pybind11_mkdoc -o bindings/python/src/docstrings.h -I include -std=c++17 -I../build/include  include/amitex/*.hpp include/amitex/input/*.hpp
