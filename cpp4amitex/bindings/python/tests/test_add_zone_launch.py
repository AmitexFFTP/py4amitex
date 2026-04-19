from pytest import approx, mark
from py4amitex.input import Input

from .mat2zone2_prepare import mat2zone2Prepare
from .utils import compareXMLFiles, testsdir

from .testutils import compareVtkWithRef, compareBinWithRef


@mark.parametrize("nbMat", [1, 2])
def test_addZoneAndLaunchThermal(nbMat: int) -> None:
    input = mat2zone2Prepare(nbMat)

    input.generateFiles()

    refDir = f"{testsdir}/ref-amxdir/addzoneandlaunchthermal_{nbMat}"
    dir = input.resultsDir

    assert compareXMLFiles(f"{dir}/materials.xml", f"{refDir}/materials.xml")
    assert compareXMLFiles(f"{dir}/algorithm.xml", f"{refDir}/algorithm.xml")
    assert compareXMLFiles(f"{dir}/loading.xml", f"{refDir}/loading.xml")

    if nbMat == 2:
        assert compareVtkWithRef(
            f"{dir}/materialIds.vtk", f"{refDir}/materialIds.vtk", 0
        )
    if nbMat == 1:
        assert compareVtkWithRef(f"{dir}/zoneIds.vtk", f"{refDir}/zoneIds.vtk", 0)

    assert compareBinWithRef(f"{dir}/CoeffK1_1.bin", f"{refDir}/CoeffK1_1.bin", 1.0e-8)
