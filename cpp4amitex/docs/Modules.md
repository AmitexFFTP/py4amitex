# Overview of headers and modules

Below a comparison of main C++ headers and corresponding python modules.

| C++ | Python | Description|
|-----|--------|------------|
| `amitex/input.hpp` | `amitex.input` | Main classes to build input data: \ref amitex::Input, \ref amitex::Grid, \ref amitex::AlgorithmParameters \ref amitex::Materials, \ref amitex::LoadingOutput |
| `amitex/input/material_builder.hpp` | `amitex.input.materialbuilder` | API to build voxel-by-voxel structure of \ref amitex::Materials, see \ref material_builder.hpp |
| `amitex/input/field.hpp` | `amitex.input.field` | Fields API, see \ref amitex::Field, and \ref Fields.md |
| `amitex/simulation.hpp` | `amitex.simulation` | Launch AMITEX simulations, see \ref simulation.hpp |