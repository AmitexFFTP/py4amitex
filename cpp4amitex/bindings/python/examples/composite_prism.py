from math import sqrt

from py4amitex.input import (
    Input,
    Grid,
    Algorithm,
    Material,
    Zone,
    ReferenceMaterial,
    Loading,
    MechanicDriving,
    Component,
    Mechanics,
    InterfaceGeometry,
    Composite,
    AlgorithmParameters,
    LoadingOutput,
    Materials,
)
from py4amitex.simulation import runSimulationExternal

"""
Sketch of material (along XY plane)
    |    /|
    | 0 / |
    |  /  |
    | / 1 |
    |/    |
"""

DIMS = [5, 5, 5]
PURECOEFFS0 = [100e6, 200e6]
PURECOEFFS1 = [150e6, 250e6]

grid = Grid(DIMS, [1.0, 1.0, 1.0])

algo = Algorithm("Basic_Scheme", convergenceAcceleration=True)
meca = Mechanics(filter="Default", smallPerturbations=True)
algoParams = AlgorithmParameters(algo, mechanics=meca)

loading = Loading()
loading.setTimeDiscretizationLinear(4, 1.0)
# Something along the interface
loading.setLinearEvolution(Component.XX, MechanicDriving.Strain, 0.01)
loading.setLinearEvolution(Component.XY, MechanicDriving.Strain, 0.01)
loading.setLinearEvolution(Component.YY, MechanicDriving.Strain, 0.01)
loading.setLinearEvolution(Component.XZ, MechanicDriving.Stress, 0.0)
loading.setLinearEvolution(Component.YZ, MechanicDriving.Stress, 0.0)
loading.setLinearEvolution(Component.ZZ, MechanicDriving.Stress, 0.0)
loadingOutput = LoadingOutput()
loadingOutput.add(loading)

zone0, zone1, zoneInter = Zone(DIMS), Zone(DIMS), Zone(DIMS)
for p in grid.allPoints():
    [ix, iy, iz] = p
    if iy > ix:
        zone0.add(p)
    elif iy == ix:
        zoneInter.add(p)
    else:
        zone1.add(p)

mat0 = Material()
mat0.setLaw("elasiso")
mat0.setNumberCoeff(2)
mat0.addZone(zone0, [PURECOEFFS0[0], PURECOEFFS1[0]])
mat0.setCoeffComposites([PURECOEFFS0[0], PURECOEFFS1[0]])
mat0.addZone(zoneInter, [PURECOEFFS0[0], PURECOEFFS1[0]])

mat1 = Material()
mat1.setLaw("elasiso")
mat1.setNumberCoeff(2)
mat1.addZone(zone1, [PURECOEFFS0[1], PURECOEFFS1[1]])
mat1.setCoeffComposites([PURECOEFFS0[1], PURECOEFFS1[1]])

materials = Materials()

materials.add(mat0)
materials.add(mat1)
materials.referenceMaterial = ReferenceMaterial(
    0.5 * (PURECOEFFS0[0] + PURECOEFFS0[1]), 0.5 * (PURECOEFFS1[0] + PURECOEFFS1[1])
)

geom = InterfaceGeometry()
geom.normal = [-1.0 / sqrt(2.0), 1.0 / sqrt(2.0), 0.0]
geom.tangent = [1.0 / sqrt(2.0), 1.0 / sqrt(2.0), 0.0]
geom.surface = sqrt(2.0)

inter = Composite([0, 1])
inter.setLaw("reuss")
for pos in zoneInter.linearPositions():
    inter.addVoxel(pos, [0.5, 0.5], [geom])
materials.add(inter)

input = Input(grid, algoParams, materials, loadingOutput)

input.resultsDir = "amitex_dir_composite_prism"

runSimulationExternal(input)
