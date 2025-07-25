# /usr/bin/env python3

# config.hpp needs to be generated before by cmake

# pip3 install pybind11_mkdoc

# if default libclang is not enough set env-var LIBCLANG_PATH=/path/to/libclang.so

from pybind11_mkdoc.mkdoc_lib import mkdoc
from pathlib import Path

outFile = "bindings/python/src/docstrings.hpp"
args = [str(path) for path in Path("include/amitex").glob("**/*.hpp")]
mkdoc(
    args
    + [
        "-Iinclude",
        "-std=c++17",
        "-I../build/include",
    ],
    200,
    outFile,
)

with open(outFile, "r", encoding="utf-8") as file:
    text = (
        file.read()
        .replace(r"\note", "**note**:")
        .replace(r"\warning", "**warning**: ")
        .replace("//!", "")
    )

with open(outFile, "w", encoding="utf-8") as file:
    file.write(text)
