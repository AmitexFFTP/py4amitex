from pytest import raises, approx

from py4amitex.input import (
    Materials,
    Input,
    ReferenceMaterial,
    Algorithm,
    AlgorithmParameters,
    LoadingOutput,
)
from py4amitex.input.materialbuilder import buildMaterialsFromVtk

from .utils import compareXMLFiles, testsdir

from .testutils import compareVtkWithRef, compareBinWithRef


def test_readvtklaminateNSB():
    materials = Materials()
    with raises(RuntimeError, match="AMITEX Input Error: Material ID"):
        buildMaterialsFromVtk(
            materials, materialIdPath=f"{testsdir}/data/laminate_NSB_n64_mate.vtk"
        )
    grid = buildMaterialsFromVtk(
        materials, materialIdPath=f"{testsdir}/data/laminate_NSB_n64_mate.vtk", minId=0
    )
    assert materials.numberMaterials() == 2
    for m in range(2):
        assert materials.material(m).numberZones() == 1
    assert grid.dims() == [1, 64, 66]
    assert grid.voxelLengths() == approx([0.001563, 0.001563, 0.001563], abs=1e-8)


def test_bilayer():
    refDir = f"{testsdir}/ref-amxdir/bilayer"
    dir = f"testresults/addzoneandlaunchbilayer"

    materials = Materials()
    materials.referenceMaterial = ReferenceMaterial(250e6, 250e6)

    grid = buildMaterialsFromVtk(
        materials,
        materialIdPath=f"{refDir}/materialIds.vtk",
        zoneIdPath=f"{refDir}/zoneIds.vtk",
        minId=1,
    )

    for m in range(2):
        mat = materials.material(m)
        mat.setLaw("elasiso", "")
        mat.setNumberCoeff(2)

        for z in range(2):
            mat.setCoeffZoneFromBin(z, f"{refDir}/Coeff{m+1}_{z+1}.bin")

    input = Input(
        grid, AlgorithmParameters(Algorithm.createDefault()), materials, LoadingOutput()
    )
    input.resultsDir = dir

    input.generateFiles()

    assert compareXMLFiles(f"{dir}/materials.xml", f"{refDir}/materials.xml")

    assert compareVtkWithRef(f"{dir}/materialIds.vtk", f"{refDir}/materialIds.vtk", 0)
    assert compareVtkWithRef(f"{dir}/zoneIds.vtk", f"{refDir}/zoneIds.vtk", 0)

    for i in range(2):
        for j in range(2):
            file = f"Coeff{i+1}_{j+1}.bin"
            assert compareBinWithRef(f"{dir}/{file}", f"{refDir}/{file}", 1.0e-8)
