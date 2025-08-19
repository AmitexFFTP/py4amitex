import os
from pathlib import Path
from shutil import copyfile

from pytest import approx, mark
from py4amitex.input import Input
from py4amitex.simulation import getSimulationShellCommandFromFiles

from .mat2zone2_prepare import mat2zone2Prepare
from .utils import compareXMLFiles, testsdir

from .testutils import compareVtkWithRef, compareBinWithRef


def test_flexible_input_paths() -> None:
    refDir = f"{testsdir}/ref-amxdir/addzoneandlaunchthermal_2"

    input = mat2zone2Prepare(2)

    oldpwd = Path.cwd()
    dir = Path(f"testresults/addzoneandlaunchthermal_2_flexible_input_paths")
    dir.mkdir(parents=True, exist_ok=True)
    os.chdir(dir)

    # We use our own "algo.xml"
    refAlgo = testsdir / "ref-xml" / "algo_no_AC.xml"
    copyfile(refAlgo, "algo.xml")

    input.generateMaterials("mat.xml", "bin")
    input.generateLoadingOutput(
        "char.xml",
    )
    input.generateMaterialVTK(f"matId.vtk")
    input.generateZoneVTK(f"zoneId.vtk")
    assert (
        getSimulationShellCommandFromFiles(
            "algo.xml", "mat.xml", "char.xml", "matId.vtk", "zoneId.vtk", "output"
        )
        == f"mpirun amitex_fftp -nm matId.vtk -nz zoneId.vtk -a algo.xml -m mat.xml -c char.xml -s output"
    )

    assert compareXMLFiles("mat.xml", f"{refDir}/materials.xml")
    assert compareXMLFiles("algo.xml", refAlgo)
    assert compareXMLFiles("char.xml", f"{refDir}/loading.xml")
    assert compareVtkWithRef(f"matId.vtk", f"{refDir}/materialIds.vtk", 0)
    assert compareVtkWithRef(f"zoneId.vtk", f"{refDir}/zoneIds.vtk", 0)
    assert compareBinWithRef(f"bin/CoeffK1_1.bin", f"{refDir}/CoeffK1_1.bin", 1.0e-8)

    os.chdir(oldpwd)


def test_gen_algo() -> None:
    refDir = f"{testsdir}/ref-amxdir/addzoneandlaunchthermal_2"

    input = mat2zone2Prepare(2)

    oldpwd = Path.cwd()
    dir = Path(f"testresults/addzoneandlaunchthermal_2_genalgo")
    dir.mkdir(parents=True, exist_ok=True)
    os.chdir(dir)

    input.generateAlgorithm("algo.xml")

    assert compareXMLFiles("algo.xml", f"{refDir}/algorithm.xml")

    os.chdir(oldpwd)
