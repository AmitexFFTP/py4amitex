from py4amitex.input import (
    Input,
    Grid,
    Algorithm,
    Mechanics,
    Material,
    Zone,
    ReferenceMaterial,
    Loading,
    Component,
    MechanicDriving,
    Evolution,
    AlgorithmParameters,
    Materials,
    LoadingOutput,
)
from py4amitex.simulation import runSimulationExternal

# 32x32x32 grid of voxels of size 1.
grid = Grid([32, 32, 32], [1.0, 1.0, 1.0])

# Set algorithm parameters with a mechanical resolution
algorithmParameters = AlgorithmParameters(
    algorithm=Algorithm(type="Basic_Scheme", convergenceAcceleration=True),
    mechanics=Mechanics(filter="Default", smallPerturbations=True),
)

materials = Materials()
# Define one elastic material
material = Material()
material.setLaw("elasiso")
# Lamé coefficients
material.setCoeffs([1.0e9, 1.5e9])
# Define the material on all voxels of the unit cell
zone = Zone(grid.dims())
for p in grid.allPoints():
    zone.add(p)
material.addZone(zone)
materials.add(material)
# Lamé coefficients of the reference material
materials.referenceMaterial = ReferenceMaterial(1.0e9, 1.5e9)
# Define one loading
loading = Loading()
# time between 0 and 1., discretized in 2 steps;
loading.setTimeDiscretizationLinear(2, 1.0)
# Impose a strain along the zz plane, evolving to 0.01 until the final time
loading.setEvolution(Component.ZZ, MechanicDriving.Strain, Evolution.Linear, 0.01)
# Relax other directions
for i in range(3):
    for j in range(i, 3):
        if i != Component.Z or j != Component.Z:
            loading.setEvolution((i, j), MechanicDriving.Stress, Evolution.Linear, 0.01)
loadingOutput = LoadingOutput()
loadingOutput.add(loading)

# Define all input categories
input = Input(grid, algorithmParameters, materials, loadingOutput)
# Define output.std in "amitex_dir"
input.resultsDir = "amitex_dir_alt"

# Run amitex_fftp on two processes
runSimulationExternal(input, numberProcs=2)
