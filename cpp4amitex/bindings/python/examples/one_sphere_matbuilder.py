
from py4amitex.input import (
    Input,
    Grid,
    Algorithm,
    Diffusion,
    Material,
    Zone,
    ReferenceMaterialD,
    Loading,
    DiffusionDriving,
    Evolution,
    LoadingOutput,
    AlgorithmParameters,
    Materials,
)
from py4amitex.input.materialbuilder import MaterialBuilder
from py4amitex.simulation import runSimulationExternal

DL = 0.0003125
NV = 32
R = 0.3 * DL * NV
center = [NV // 2, NV // 2, NV // 2]
kappas = [0.6, 429.0]

input = Input()
grid = Grid([NV, NV, NV], [DL, DL, DL])
input.grid = grid

materials = Materials()

materials.referenceMaterialD = ReferenceMaterialD(214.8)

for m in range(2):
    material = Material()
    material.setLawK("Fourier_iso_polarization")
    material.setCoeffKs([kappas[0], 0.0, 0.0, -kappas[0]])
    materials.add(material)

zone0, zone1 = Zone(grid.dims()), Zone(grid.dims())
matBuilder = MaterialBuilder()
for p in grid.allPoints():
    numMat = 1 if grid.distance(p, center) < R else 0
    matBuilder.addVoxel(materials, grid.linearize(p), numMat)


algorithm = Algorithm(type="Basic_Scheme", convergenceAcceleration=True)
algorithm.convergenceCriterion = 1.0e-4
algorithm.nitermax = 3000

diffusion = Diffusion(filter="Default", stationary=True)
algorithmParameters = AlgorithmParameters(algorithm, diffusion=diffusion)

loadingOutput = LoadingOutput()
loadingOutput.output.setVtkFluxDGradD(1, 1)

loading = Loading()
loading.setTimeDiscretizationLinear(1, 1.0)
for i in range(3):
    loading.setEvolution(i, DiffusionDriving.Gradient, Evolution.Linear, 0.0)
loading.setOutputVtkList([1])
loadingOutput.add(loading)

input = Input(grid, algorithmParameters, materials, loadingOutput)
input.resultsDir = "amitex_dir_one_sphere"

runSimulationExternal(input)