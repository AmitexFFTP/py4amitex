from pathlib import Path

from py4amitex.input import (
    Input,
    Grid,
    Algorithm,
    AlgorithmParameters,
    Diffusion,
    Materials,
    Material,
    DiffusionDriving,
    Evolution,
    Loading,
    LoadingOutput,
    Output,
    Zone,
    ReferenceMaterialD,
)

from py4amitex.input.materialbuilder import buildMaterials, VoxelSpec, IndexOrdering
from py4amitex.simulation import getSimulationShellCommand

from .utils import compareTextFiles, testsdir


# Define algorithm
def makeAlgoParams():
    # Set algorithm parameters
    algorithm = Algorithm(type="Basic_Scheme", convergenceAcceleration=True)
    # Define a mechanical resolution
    diffusion = Diffusion(filter="Default", stationary=True)
    algorithmParameters = AlgorithmParameters(algorithm, diffusion=diffusion)
    return algorithmParameters


# Define loading
def makeLoadings():
    loadingOutput = LoadingOutput()

    loading = Loading()
    # 1 timesteps to final time 1
    loading.setTimeDiscretizationLinear(1, 1.0)
    # Applay strain for XX,YY,ZZ components
    for i in range(3):
        loading.setEvolution(
            i, DiffusionDriving.Gradient, Evolution.Linear, 1.0 if i == 0 else 0.0
        )
    # Output
    loading.setOutputVtkList([1])
    loadingOutput.add(loading)
    output = Output()
    output.setVtkFluxDGradD(fluxD=1, gradD=1)
    loadingOutput.output = output
    #
    return loadingOutput


def makeVoxels1ZonePerVoxel(grid):
    voxels = []
    for izone, p in enumerate(grid.allPoints()):
        voxels.append(VoxelSpec([(0, 1.0, izone)]))
    return voxels


def makeVoxels1Mat(grid):
    voxels = []
    for i, _ in enumerate(grid.allPoints()):
        voxels.append(VoxelSpec([(0, 1.0, i % 2)]))
    return voxels


def makeVoxels1Zone(grid):
    voxels = []
    for i, _ in enumerate(grid.allPoints()):
        voxels.append(VoxelSpec([(i % 2, 1.0, 0)]))
    return voxels


# Materials
def makeMaterials(grid, voxels):
    materials = Materials()
    materials.referenceMaterialD = ReferenceMaterialD(2.0)

    buildMaterials(materials, grid.dims(), voxels)

    for m in range(materials.numberMaterials()):
        mat = materials.material(m)
        mat.setLawK("Fourier_iso_polarization")
        mat.setCoeffKs([2.0e-2, 0, 0, -2.0e-2])

    return materials


GRID_DIMS = [4, 8, 16]
L = [1.0] * 3


def test_1voxelPerZone():
    algorithmParameters = makeAlgoParams()
    loadingOutput = makeLoadings()
    grid_amitex = Grid(GRID_DIMS, [L[i] / GRID_DIMS[i] for i in range(3)])
    materials = makeMaterials(grid_amitex, makeVoxels1ZonePerVoxel(grid_amitex))
    input = Input(grid_amitex, algorithmParameters, materials, loadingOutput)
    input.resultsDir = "testresults/noVTK1voxelPerZone"
    input.generateFiles()
    assert (
        getSimulationShellCommand(input, 16)
        == f"mpirun -n 16 amitex_fftp -a {input.resultsDir}/algorithm.xml -m {input.resultsDir}/materials.xml -c {input.resultsDir}/loading.xml -s {input.outputPrefix()} -NX 4 -NY 8 -NZ 16 -DX 0.250000 -DY 0.125000 -DZ 0.062500"
    )
    assert not Path(f"{input.resultsDir}/materialIds.vtk").exists()
    assert not Path(f"{input.resultsDir}/zoneIds.vtk").exists()

    refDir = testsdir / "ref-amxdir" / "noVTK1voxelPerZone"
    dir = Path(input.resultsDir)
    assert compareTextFiles(dir / "commands.in", refDir / "commands.in")


def test_1mat():
    algorithmParameters = makeAlgoParams()
    loadingOutput = makeLoadings()
    grid_amitex = Grid(GRID_DIMS, [L[i] / GRID_DIMS[i] for i in range(3)])
    materials = makeMaterials(grid_amitex, makeVoxels1Mat(grid_amitex))
    input = Input(grid_amitex, algorithmParameters, materials, loadingOutput)
    input.resultsDir = "testresults/noVTK1mat"
    input.generateFiles()
    assert (
        getSimulationShellCommand(input, 16)
        == f"mpirun -n 16 amitex_fftp -nz {input.resultsDir}/zoneIds.vtk -a {input.resultsDir}/algorithm.xml -m {input.resultsDir}/materials.xml -c {input.resultsDir}/loading.xml -s {input.outputPrefix()}"
    )
    assert Path(f"{input.resultsDir}/zoneIds.vtk").exists()
    assert not Path(f"{input.resultsDir}/materialIds.vtk").exists()

    refDir = testsdir / "ref-amxdir" / "noVTK1mat"
    dir = Path(input.resultsDir)
    assert compareTextFiles(dir / "commands.in", refDir / "commands.in")


def test_1zone():
    algorithmParameters = makeAlgoParams()
    loadingOutput = makeLoadings()
    grid_amitex = Grid(GRID_DIMS, [L[i] / GRID_DIMS[i] for i in range(3)])
    materials = makeMaterials(grid_amitex, makeVoxels1Zone(grid_amitex))
    input = Input(grid_amitex, algorithmParameters, materials, loadingOutput)
    input.resultsDir = "testresults/noVTK1zone"
    input.generateFiles()
    assert (
        getSimulationShellCommand(input, 16)
        == f"mpirun -n 16 amitex_fftp -nm {input.resultsDir}/materialIds.vtk -a {input.resultsDir}/algorithm.xml -m {input.resultsDir}/materials.xml -c {input.resultsDir}/loading.xml -s {input.outputPrefix()}"
    )
    assert not Path(f"{input.resultsDir}/zoneIds.vtk").exists()
    assert Path(f"{input.resultsDir}/materialIds.vtk").exists()

    refDir = testsdir / "ref-amxdir" / "noVTK1zone"
    dir = Path(input.resultsDir)
    assert compareTextFiles(dir / "commands.in", refDir / "commands.in")
