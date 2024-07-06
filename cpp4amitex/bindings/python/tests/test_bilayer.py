from pytest import approx

from py4amitex.input import (
    Mechanics,
    Loading,
    MechanicDriving,
    Material,
    Component,
    Zone,
    Input,
    Grid,
    Component,
    ReferenceMaterial,
    Algorithm,
    AlgorithmParameters,
    Materials,
    LoadingOutput,
)

from .utils import compareXMLFiles, testsdir

from .testutils import compareVtkWithRef, compareBinWithRef


def makeRectangularZone(begin, end, dims):
    return Zone(
        dims,
        [
            (ix, iy, iz)
            for ix in range(begin[0], end[0])
            for iy in range(begin[1], end[1])
            for iz in range(begin[2], end[2])
        ],
    )


def test_bilayer():
    N = 40
    DX = 0.025
    LAMBDA = [[100.0e6, 200.0e6], [300.0e6, 400.0e6]]
    MU = [[100.0e6, 200.0e6], [300.0e6, 400.0e6]]
    grid = Grid([N, N, N], [DX, DX, DX])

    meca = Mechanics(filter="Default", smallPerturbations=True)
    algoParams = AlgorithmParameters(
        Algorithm(type="Basic_Scheme", convergenceAcceleration=True), mechanics=meca
    )

    loading = Loading()
    loading.setTimeDiscretizationLinear(1, 100.0)
    loading.setLinearEvolution(Component.XX, MechanicDriving.Strain, 0.01)
    for i in range(3):
        for j in range(i, 3):
            if i != 0 or j != 0:
                loading.setLinearEvolution((i, j), MechanicDriving.Stress, 0.0)
    loadingOutput = LoadingOutput()
    loadingOutput.add(loading)

    materials = Materials()
    materials.referenceMaterial = ReferenceMaterial(250e6, 250e6)

    for m in range(2):
        mat = Material()
        mat.setLaw("elasiso", "")
        mat.setNumberCoeff(2)

        minY = N // 2 if m == 0 else 0
        maxY = N if m == 0 else N // 2

        for z in range(2):
            minZ = N // 2 if z == 0 else 0
            maxZ = N if z == 0 else N // 2
            zone = makeRectangularZone((0, minY, minZ), (N, maxY, maxZ), (N, N, N))
            print(zone.numberVoxels())
            mat.addZone(zone, [LAMBDA[m][z], MU[m][z]])

        materials.add(mat)

    input = Input(grid, algoParams, materials, loadingOutput)
    input.resultsDir = "testresults/addzoneandlaunchbilayer"

    input.generateFiles()

    refDir = f"{testsdir}/ref-amxdir/bilayer"
    dir = input.resultsDir

    assert compareXMLFiles(f"{dir}/materials.xml", f"{refDir}/materials.xml")
    assert compareXMLFiles(f"{dir}/algorithm.xml", f"{refDir}/algorithm.xml")
    assert compareXMLFiles(f"{dir}/loading.xml", f"{refDir}/loading.xml")

    assert compareVtkWithRef(f"{dir}/materialIds.vtk", f"{refDir}/materialIds.vtk", 0)
    assert compareVtkWithRef(f"{dir}/zoneIds.vtk", f"{refDir}/zoneIds.vtk", 0)

    for i in range(2):
        for j in range(2):
            file = f"Coeff{i+1}_{j+1}.bin"
            assert compareBinWithRef(f"{dir}/{file}", f"{refDir}/{file}", 1.0e-8)
