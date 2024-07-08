
# Mérope examples

## Prerequisite

Environment variable:
	- CMAKE_PREFIX_PATH pointing to /py4amitex-install-dir/lib/cmake
	- MEROPE_ROOT_DIR set to a Mérope source root directory
	- MEROPE_INSTALL_DIR set to a Mérope install root directory (for ex $MEROPE_ROOT_DIR/INSTALL-DIR)

## Build

```
cmake -S. -Bbuild-dir
cmake --build build
```

Executables are in build-dir.

