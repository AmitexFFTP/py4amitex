
# Step by step example

A simple mechanical example.

## Import

Let us start by importing the basic input classes

```py
from amitex.input import (
    Input,
    Grid,
    AlgorithmParameters,
    Materials,
    LoadingOutput
)
```

`Input` contains all parameters, `Grid` specify the grid dimensions, the last three correspond to
the root nodes of AMITEX XML input files. 

If not specified, all classes introduces below come from `amitex.input`.

## Grid

Let us make a cell of 32x32x32, with voxels of side 1
```py
grid = Grid([32, 32, 32], [1.0, 1.0, 1.0])
```

## Algorithm

The basic algorithm parameters are defined in "node" Algorithm (importable from amitex.input), we need to set up the algorithm type and the convergence acceleration.
```py
algorithm = Algorithm(type="Basic_Scheme", convergenceAcceleration=True)
```

Parameters can be set, for example to set the maximum number of iterations:
```py
algorithm.nitermax = 3000
```

Most parameters can be set in the constructor, like:
```py
algorithm = Algorithm(type="Basic_Scheme", convergenceAcceleration=True, nitermax=3000)
```

To set up a mechanical resolution, we nee to specify the filter and small perturbations of the Mechanics node:
```py
mechanics = Mechanics(filter="Default", smallPerturbations=True)
```

Then, we gather all algorithm parameters:
```
algorithmParameters = AlgorithmParameters(algorithm, mechanics=mechanics)
```

Alternatively, one could write
```py
algorithmParameters = AlgorithmParameters(algorithm)
algorithmParameters.mechanics = mechanics
```

The corresponding XML is
```xml
<Algorithm_Parameters>
        <Algorithm Type="Basic_Scheme">
                <Convergence_Criterion Value="Default"/>
                <Convergence_Acceleration Value="True"/>
        </Algorithm>
        <Mechanics>
                <Filter Type="Default"/>
                <Small_Perturbations Value="True"/>
        </Mechanics>
</Algorithm_Parameters>
```

## Materials

Fist we need to instantiate the `Materials` class regrouping all material specification.
```py
materials = Materials()
```

We have one isotropic elastic material
```py
material = Material()
material.setLaw("elasiso")
```

We need to set the 2 coefficients for this material:
```py
material.setCoeffs([1.0e9, 1.5e9])
```

To place a material, one has to define its zones, which are no more than a list of positions.
Here a zone is defined on the whole cell:
```python3
zone = Zone(grid.dims())
for p in grid.allPoints():
    zone.add(p)
```
A zone is initialized with the dimensions of the grid (we use here `grid.dims()` with `grid` defined above). `grid.allPoints()` iterates over all points in the cell, each point is added.

A zone can be added to the material with
```py
material.addZone(zone)
```

Note that `addZone` accepts more parameters to associate coefficients to a zone, for example:
```py
material.addZone(zone, coeffs=[1.0e9, 1.5e9])
```

We can now add the material to the list of materials:
```py
materials.add(material)
```

Finally, we need to set up the reference material:
```py
materials.referenceMaterial = ReferenceMaterial(1.0e9, 1.5e9)
```

The resulting XML is
```xml
<Materials>
        <Reference_Material Lambda0="1e+09" Mu0="1.5e+09"/>
        <Material numM="1" Law="elasiso" Lib="">
                <Coeff Index="1" Type="Constant" Value="1e+09"/>
                <Coeff Index="2" Type="Constant" Value="1.5e+09"/>
        </Material>
</Materials>
```

## Loading & Outputs

```py
loadingOutput = LoadingOutput()
```

### Output

Le us just switch on stress and strain VTK outputs
```py
output = Output()
output.setVtkStressStrain(1, 1)
loadingOutput.output = output
```

### Loading(s)

We will set up a loading ending at time 1.0 with 2 steps, with the ZZ component of the finite strain set to 0.01, and all other components will be relaxed

```py
loading = Loading()
loading.setTimeDiscretizationLinear(2, 1.0)
```

The evolution of the strain component ZZ towards 0.01 can be set with:
```
loading.setEvolution(Component.ZZ, MechanicDriving.Strain, Evolution.Linear, 0.01)
```
Component contains values XX, YY, ZZ, XY… . These are tuples (I. J) where I or J are equal to 0,1,2, `XX = (0, 0)`,  `XX = (0, 1)` etc…
MechanicDriving is an enum of values `Strain` and `Stress`, `Evolution` can be `Linear` or `Constant` (in the constant case, specifying a value as last parameter is not needed).
This allow to set component from loops:
```py
for i in range(3):
    for j in range(i, 3):
        if i != Component.Z or j != Component.Z:
            loading.setEvolution((i, j), MechanicDriving.Stress, Evolution.Linear, 0.0)
```
Here only (i,j) != ZZ and with j >= i are considered, because we are in small perturbations (for more on this the reader is referred to the official AMITEX documentation).

Then we add the loading to `loadingOutput`:
```py
loadingOutput.add(loading)
```

### Corresponding XML

The resulting XML is
```xml
<Loading_Output>
        <Output>
                <vtk_StressStrain Strain="1" Stress="1"/>
        </Output>
        <Loading Tag="1">
                <Time_Discretization Discretization="Linear" Nincr="2" Tfinal="1"/>
                <zz Driving="Strain" Evolution="Linear" Value="0.01"/>
                <xx Driving="Stress" Evolution="Linear" Value="0"/>
                <xy Driving="Stress" Evolution="Linear" Value="0"/>
                <xz Driving="Stress" Evolution="Linear" Value="0"/>
                <yy Driving="Stress" Evolution="Linear" Value="0"/>
                <yz Driving="Stress" Evolution="Linear" Value="0"/>
        </Loading>
</Loading_Output>
```

## Input

It is now time to gather all pieces of input parameters wih the `Input` class
```py
input = Input(grid, algorithmParameters, materials, loadingOutput)
```

This library puts all generated files to a directory (by default "amitex_dir"), it can be set with:
```py
input.resultsDir = "my_amitex_dir"
```

Output files in a subdirectory `output` in this directory. Moreover all output filenames will
have the prefix `output`. The total prefix can be queried with:
```py
input.outputPrefix()
```

## Run simulation

We can now run a simulation with AMITEX-FFTP
```py
from amitex.simulation import runSimulationExternal
runSimulationExternal(input, numberProcs=2)
```
This will generate the necessary files and execute `amitex_fftp`.
The second optional parameter sets the number of MPI processes used for `amitex_fftp`. By default it is set at 0, which (likely) means the number of cores on your machine will be used (`mpirun` is executed without `-n`.)

### Separate generation and running steps

One could decompose the steps by first generating the input files and then generate a script from
the command that would be called by `runSimulationExternal`.
```py
from amitex.simulation import getSimulationShellCommand
input.generateFiles()
with open("run.sh", "w", encoding="utf-8") as file:
    file.write("#!/bin/sh\n")
    file.write(getSimulationShellCommand(input, numberProcs=2) + "\n")
```

## Full example

```{literalinclude} ../examples/Input/minimal.py
```
