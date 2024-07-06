import math as m

import sac_de_billes
import merope

from py4amitex.input import (
    Input,
    Grid,
    Algorithm,
    AlgorithmParameters,
    Mechanics,
    Materials,
    Material,
    MechanicDriving,
    Evolution,
    Loading,
    LoadingOutput,
    Zone,
    ReferenceMaterial,
)
from py4amitex.input.materialbuilder import buildMaterials, VoxelSpec, IndexOrdering
from py4amitex.simulation import runSimulationExternal

"""
Generation of a polycrystal with Mérope, then preparation and lauch of an AMITEX-FFTP
simulation of an iso-elastic material with
coefficients for each microcrystal. This showcase the the API `buildMaterials` witch take
as input a 'field' of per-voxel data (material, volume fraction, + in option zone index)
A small intergation example pyth p4amitex is show at the end (optional).

Two approches for N crystals:
    - N materials, 1 zone per material (makeMaterials)
    - 1 material, N zones (makeMaterialWithZone)

Moreover, Mérope can generate optionnally (flag --composite) composite voxels, wich are translated 
into AMITEX-FFTP composite voxels ('reuss' law here), after filtration of low fraction phases.

**Caveat**:

Using the 1 material/N zones with composite materials does not really work, because the composite materials are nominally
composed of the same material, one would have to somehow associate phases of the same AMITEX material
with a different zone.
"""

L = [2, 2, 2]
N3D = 32
GRID_DIMS = [N3D, N3D, N3D]


def makeStruture(composite):
    # lifted from merope_bibliotheque/tests/microstructures/polyCrystal/polyCrystal.py
    seed = 0
    spheres = sac_de_billes.throwSpheres_3D(
        sac_de_billes.TypeAlgo.RSA,
        sac_de_billes.NameShape.Tore,
        L,
        seed,
        [[0.5, 1]],
        [1],
        0,
    )

    mIncl = merope.SphereInclusions_3D()
    mIncl.setLength(L)
    mIncl.setSpheres(spheres)

    polyCrystal = merope.LaguerreTess_3D(L, spheres)
    multiInclusions = merope.MultiInclusions_3D()
    multiInclusions.setInclusions(polyCrystal)
    multiInclusions.changePhase(
        multiInclusions.getAllIdentifiers(), multiInclusions.getAllIdentifiers()
    )

    grid = merope.Voxellation_3D(multiInclusions)

    if composite:
        grid.setVoxelRule(merope.VoxelRule.Average)
    else:
        grid.setVoxelRule(merope.VoxelRule.Center)
    return grid.computePhaseGrid(GRID_DIMS)


def coeffs(iphase):
    return (1.0 + 0.1 * m.cos(iphase * 2.0), 1.5 + 0.1 * m.sin(iphase * 2.0))


def makeMaterials(phaseGrid):
    materials = Materials()
    nPhases = 1 + max(p[0][0] for p in phaseGrid)

    for iph in range(nPhases):
        mat = Material()
        mat.setLaw("elasiso")
        mat.setCoeffs(coeffs(iph))
        mat.setCoeffComposites(coeffs(iph))
        materials.add(mat)

    # In this case the structure of computePhaseGrid is directly pluggable into buildMaterials
    buildMaterials(materials, GRID_DIMS, phaseGrid, ordering=IndexOrdering.C)
    return materials


def makeMaterialWithZone(phaseGrid):

    materials = Materials()
    nPhases = 1 + max(p[0][0] for p in phaseGrid)
    mat = Material()
    mat.setLaw("elasiso")
    mat.setNumberCoeff(2)
    mat.setNumberCoeffComposite(2)
    for iph in range(nPhases):
        # Empty zone, to be filled by buildMaterials
        mat.addZone(Zone(GRID_DIMS), coeffs=coeffs(iph), coeffComposites=coeffs(iph))
    materials.add(mat)
    # A bit more involved with a type VoxelSpec with zone specifications:
    # every one is material 0, but phase index is taken as a zone index
    # Schematically:  [(phaseId,Φ)] -> [(0,Φ,zoneId=phaseId)]
    voxels = [VoxelSpec([(0, p[1], p[0]) for p in vox]) for vox in phaseGrid]
    buildMaterials(materials, GRID_DIMS, voxels, ordering=IndexOrdering.C)
    return materials


def makeAlgoParams():
    algo = Algorithm(type="Basic_Scheme", convergenceAcceleration=True)
    meca = Mechanics(filter="Default", smallPerturbations=True)
    return AlgorithmParameters(algo, mechanics=meca)


def makeLoadings():
    loadingOutput = LoadingOutput()

    loading = Loading()
    # 10 timesteps to final time 1
    loading.setTimeDiscretizationLinear(10, 1.0)
    # Applay strain for XX,YY,ZZ components
    for i in range(3):
        loading.setEvolution((i, i), MechanicDriving.Strain, Evolution.Linear, 0.01)
    for i in range(3):
        for j in range(i + 1, 3):
            loading.setEvolution((i, j), MechanicDriving.Stress, Evolution.Linear, 0.0)

    loadingOutput.add(loading)
    return loadingOutput


if __name__ == "__main__":
    from argparse import ArgumentParser

    parser = ArgumentParser(
        description="AMITEX simulation of a  Mérope-generated polycrystal"
    )
    parser.add_argument("--composite", action="store_true", help="Use composite voxels")
    parser.add_argument(
        "--zones",
        action="store_true",
        help="Use N zones instead of N materials with 1 zone",
    )
    args = parser.parse_args()

    phaseGrid = makeStruture(composite=args.composite)
    nPhases = 1 + max(p[0][0] for p in phaseGrid)
    nCompoMax = max(len(p) for p in phaseGrid)
    nCompoVox = sum(1 for p in phaseGrid if len(p) > 1)
    print(f"#phases = {nPhases}  max-#phases/voxel = {nCompoMax}")
    print(f"#composite-voxels = {nCompoVox}")

    grid = Grid(GRID_DIMS, [a / N3D for a in L])
    algoParams = makeAlgoParams()
    loadings = makeLoadings()
    if args.zones:
        materials = makeMaterialWithZone(phaseGrid)
    else:
        materials = makeMaterials(phaseGrid)
    materials.referenceMaterial = ReferenceMaterial(1.0, 1.5)
    for c in range(materials.numberComposites()):
        materials.composite(c).setLaw("laminate")

    input = Input(grid, algoParams, materials, loadings)
    input.resultsDir = "amitex_merope_poly"

    runSimulationExternal(input, numberProcs=1)

    try:
        from p4am.output.amitexoutput import AmitexOutput

        output = AmitexOutput(input.outputPrefix())
        output.load_mean_values()
        sig = output.get_mean_values("stress")
        print(sig[-1])
    except ModuleNotFoundError:
        print("Warning: module 'p4am' not found. Update PYTHONPATH ?")
