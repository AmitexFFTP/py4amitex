from math import exp

from pytest import approx

from py4amitex.input import (
    Algorithm,
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
    Composite,
    InterfaceGeometry,
    AlgorithmParameters,
    LoadingOutput,
    Materials,
)
from py4amitex.input.materialbuilder import (
    MaterialBuilder,
    VoxelSpec,
    IndexOrdering,
    buildMaterials,
)
from py4amitex.simulation import runSimulationExternal
from py4amitex.output.amitexoutput import AmitexOutput

"""
Sketch of phase 1 Φ (along XY plane)
    |...……...|
    |...……...|
    |...……...|
    |...……...|
"""

PURE_COEFFS0 = [100e6, 200e6]
PURE_COEFFS1 = [100e6, 200e6]

DIMS = [32, 32, 32]
VOXL = [1.0, 1.0, 1.0]


def makeInputCommon():
    grid = Grid(DIMS, VOXL)
    algo = Algorithm(type="Basic_Scheme", convergenceAcceleration=True)
    meca = Mechanics(filter="Default", smallPerturbations=True)
    algoParams = AlgorithmParameters(algo, mechanics=meca)

    loading = Loading()
    loading.setTimeDiscretizationLinear(4, 1.0)
    loading.setLinearEvolution(Component.XX, MechanicDriving.Strain, 0.01)
    loading.setLinearEvolution(Component.XY, MechanicDriving.Strain, 0.01)
    loading.setLinearEvolution(Component.YY, MechanicDriving.Strain, 0.01)
    loading.setLinearEvolution(Component.XZ, MechanicDriving.Stress, 0.0)
    loading.setLinearEvolution(Component.YZ, MechanicDriving.Stress, 0.0)
    loading.setLinearEvolution(Component.ZZ, MechanicDriving.Stress, 0.0)
    lo = LoadingOutput()
    lo.add(loading)
    return (grid, algoParams, lo)


def getPhasesVolFrac(p):
    L = DIMS[0] * VOXL[0]
    x = p[0] * VOXL[0]
    x0 = 0.5 * L
    l0 = 0.1 * L
    f = (x - x0) / l0
    phi1 = 0.95 * exp(-0.5 * f * f)
    phi = [1.0 - phi1, phi1]
    assert 1.0 >= phi[1] >= 0.0

    pids = [i for i, p in enumerate(phi) if p > 1.0e-3]
    vfs = [p for p in phi if p > 1.0e-3]
    return pids, vfs


def buildStructureManual(materials, law):
    zone0, zone1, zoneInter = (Zone(DIMS), Zone(DIMS), Zone(DIMS))
    geom = InterfaceGeometry()
    geom.normal = [-1, 0.0, 0.0]
    geom.tangent = [0, 1.0, 0.0]
    geom.surface = VOXL[1] * VOXL[2]
    inter = Composite((0, 1), law)
    grid = Grid(DIMS, VOXL)
    for point in grid.allPoints():
        pids, vfs = getPhasesVolFrac(point)
        lpos = grid.linearize(point)
        if len(pids) == 1:
            if pids[0] == 0:
                zone0.add(lpos)
            else:
                zone1.add(lpos)
        else:
            inter.addVoxel(lpos, vfs, [geom])
            zoneInter.add(lpos)

    materials.add(inter)
    materials.material(0).addZone(zone0)
    materials.material(1).addZone(zone1)

    materials.material(0).addZone(zoneInter)
    # materials.material(1).addZone(zoneInter)


def getPhaseOfTupleList(point):
    pids, vfs = getPhasesVolFrac(point)
    return [(pid, vf) for pid, vf in zip(pids, vfs)]


def buildStructureMatBuilder(grid, materials, law):
    matbd = MaterialBuilder()
    for point in grid.allPoints():
        spec = VoxelSpec(getPhaseOfTupleList(point))
        lpos = grid.linearize(point)
        matbd.addVoxel(materials, lpos, spec, normal=[-1, 0.0, 0.0])
    materials.composite(0).setLaw(law)


def buildStructureBuildMat(grid, materials, law):

    phases = [
        (VoxelSpec(getPhaseOfTupleList(point)), [-1.0, 0, 0])
        for point in grid.allPoints()
    ]
    buildMaterials(materials, grid.dims(), phases, ordering=IndexOrdering.Fortran)
    materials.composite(0).setLaw(law)


def compositeContinu(law, useBuilder=0):
    grid, algoParams, loadingOutput = makeInputCommon()

    materials = Materials()
    materials.referenceMaterial = ReferenceMaterial(
        0.5 * (PURE_COEFFS0[0] + PURE_COEFFS0[1]),
        0.5 * (PURE_COEFFS1[0] + PURE_COEFFS1[1]),
    )
    for m in range(2):
        mat = Material()
        mat.setLaw("elasiso")
        mat.setCoeffs([PURE_COEFFS0[m], PURE_COEFFS1[m]])
        mat.setCoeffComposites([PURE_COEFFS0[m], PURE_COEFFS1[m]])
        materials.add(mat)

    if useBuilder == 1:
        buildStructureMatBuilder(grid, materials, law)
    elif useBuilder == 2:
        buildStructureBuildMat(grid, materials, law)
    else:
        buildStructureManual(materials, law)

    input = Input(grid, algoParams, materials, loadingOutput)
    input.resultsDir = f"testresults/amitex_testcompositeconti_{law}"

    runSimulationExternal(input, numberProcs=2)

    output = AmitexOutput(input.outputPrefix())
    output.load_mean_values()
    stress = output.get_mean_values("stress")
    return stress[-1]


def compare():
    stressCompReuss = compositeContinu("reuss", 1)
    stressCompVoigt = compositeContinu("voigt", 0)
    stressCompLaminate = compositeContinu("laminate", 2)

    print("Reuss\tVoigt\tLaminate")
    for value in zip(stressCompReuss, stressCompVoigt, stressCompLaminate):
        print(f"{value[0]:16.7e} {value[1]:16.7e} {value[2]:16.7e}")


if __name__ == "__main__":
    compare()
