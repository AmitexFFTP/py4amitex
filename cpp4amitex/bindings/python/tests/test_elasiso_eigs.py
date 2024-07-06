import subprocess

from pytest import approx

from py4amitex.input import (
    Algorithm,
    AlgorithmParameters,
    Mechanics,
    LoadingOutput,
    Loading,
    MechanicDriving,
    Material,
    Materials,
    Component,
    Zone,
    Input,
    Grid,
    Component,
    ReferenceMaterial,
)
from py4amitex.input.field import FieldDouble

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


def test_elasiso_eigs():
    grid = Grid([32, 32, 32], [1.0, 1.0, 1.0])

    algorithm = Algorithm(type="Basic_Scheme", convergenceAcceleration=True)
    mechanics = Mechanics(filter="Default", smallPerturbations=True)
    algoParams = AlgorithmParameters(algorithm, mechanics=mechanics)
    algoParams.algorithm.convergenceAcceleration = False

    loadingOutput = LoadingOutput()
    loading = Loading()
    loading.setTimeDiscretizationLinear(10, 1.0e-11)
    loading.setLinearEvolution(Component.XX, MechanicDriving.Stress, 20.0e6)
    for i in range(3):
        for j in range(3):
            if i != 0 or j != 0:
                loading.setLinearEvolution((i, j), MechanicDriving.Stress, 0.0)
    loadingOutput.add(loading)

    materials = Materials()
    lambd, mu = (26.235e10, 42.00e09)
    materials.referenceMaterial = ReferenceMaterial(lambd, mu)
    material = Material()
    material.setLaw("elasiso_eigs")
    material.setCoeffs([lambd, mu])
    freeStr0 = FieldDouble(grid.dims())
    freeStr0.fill(0.0)
    material.addIntVar(freeStr0)
    for i in range(1, 6):
        material.addIntVar(0)
    zone = Zone(grid.dims())
    zone.add(grid.allPoints())
    material.addZone(zone)
    materials.add(material)

    input = Input(grid, algoParams, materials, loadingOutput)
    input.resultsDir = "testresults/elaiso_eigs"

    input.generateFiles()

    refDir = f"{testsdir}/ref-amxdir/elaiso_eigs"
    dir = input.resultsDir

    assert compareXMLFiles(f"{dir}/materials.xml", f"{refDir}/materials.xml")
    assert compareXMLFiles(f"{dir}/algorithm.xml", f"{refDir}/algorithm.xml")
    assert compareXMLFiles(f"{dir}/loading.xml", f"{refDir}/loading.xml")

    assert compareVtkWithRef(f"{dir}/intvar_1_1.vtk", f"{refDir}/intvar_1_1.vtk", 0)
