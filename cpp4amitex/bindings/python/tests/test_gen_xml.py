import pytest
from py4amitex.input import (
    Algorithm,
    AlgorithmLaminate,
    AlgorithmParameters,
    Component,
    ConvergenceAcceleration,
    ConvergenceForced,
    Diffusion,
    DiffusionDriving,
    DirStress,
    Evolution,
    InitLoadExt,
    Loading,
    LoadingOutput,
    Material,
    Materials,
    MechanicDriving,
    Mechanics,
    Output,
    ReferenceMaterial,
    ReferenceMaterialD,
    Substepping,
    VtkFluxDGradD,
    VtkStressStrain,
)

from .utils import compareXML, testsdir


def compareXMLWithRef(obj, refFilePath):
    with open(f"{testsdir}/ref-xml/" + refFilePath, encoding="utf-8") as refFile:
        return compareXML(obj.toXML(), refFile.read())


def test_XML_AlgoDefault():
    algo = Algorithm.createDefault()
    algo.type = "Basic_Scheme"
    algo.convergenceAcceleration = False
    algo.convergenceAcceleration.value = True

    meca = Mechanics.createDefault()
    meca.filter = "Default"
    meca.smallPerturbations = True

    param_algo = AlgorithmParameters(Algorithm.createDefault())
    param_algo.algorithm = algo
    param_algo.mechanics = meca

    assert compareXMLWithRef(param_algo, "algo_default.xml")


def test_XML_AlgoDefault_Declarative():
    param_algo = AlgorithmParameters(
        algorithm=Algorithm(type="Basic_Scheme", convergenceAcceleration=True),
        mechanics=Mechanics(filter="Default", smallPerturbations=True),
    )

    assert compareXMLWithRef(param_algo, "algo_default.xml")


def test_XML_AlgoNoAc():
    algo = Algorithm.createDefault()
    algo.type = "Basic_Scheme"
    algo.convergenceAcceleration = False

    meca = Mechanics.createDefault()
    meca.filter = "Default"
    meca.smallPerturbations = True

    param_algo = AlgorithmParameters(Algorithm.createDefault())
    param_algo.algorithm = algo
    param_algo.mechanics = meca

    assert compareXMLWithRef(param_algo, "algo_no_AC.xml")


def test_XML_AlgoNoAc_Declarative():
    param_algo = AlgorithmParameters(
        algorithm=Algorithm(type="Basic_Scheme", convergenceAcceleration=False),
        mechanics=Mechanics(filter="Default", smallPerturbations=True),
    )

    assert compareXMLWithRef(param_algo, "algo_no_AC.xml")


def test_XML_AlgoDefaultCompatibility():
    algo = Algorithm(
        type="Basic_Scheme",
        nitermax=2000,
        convergenceCriterionCompatibility=1.0e-10,
        convergenceAcceleration=True,
    )
    param_algo = AlgorithmParameters(
        algorithm=algo,
        mechanics=Mechanics(filter="Default", smallPerturbations=True),
    )
    assert compareXMLWithRef(param_algo, "algo_default_compatibility.xml")


def test_XML_AlgoDefaultCVFor():
    cvfor = ConvergenceForced(value=True, nitCVFor=100, nCVFor=100)
    cvfor.nCVFor += 1
    param_algo = AlgorithmParameters(
        algorithm=Algorithm(
            type="Basic_Scheme",
            convergenceForced=cvfor,
            convergenceAcceleration=True,
        ),
        mechanics=Mechanics(filter="Default", smallPerturbations=True),
    )
    assert compareXMLWithRef(param_algo, "algo_default_cvfor.xml")


def test_XML_AlgoDefaultInitPrevious():
    param_algo = AlgorithmParameters(
        algorithm=Algorithm(type="Basic_Scheme", convergenceAcceleration=True),
        mechanics=Mechanics(filter="Default", smallPerturbations=True),
    )
    param_algo.algorithm.initialize = "previous"
    assert compareXMLWithRef(param_algo, "algo_default_initprevious.xml")


def test_XML_AlgoDefaultModACV():
    acv = ConvergenceAcceleration(value=True, modACV=2)
    param_algo = AlgorithmParameters(
        algorithm=Algorithm(type="Basic_Scheme", convergenceAcceleration=acv),
        mechanics=Mechanics(filter="Default", smallPerturbations=True),
    )
    assert compareXMLWithRef(param_algo, "algo_default_modACV.xml")


def test_XML_AlgoDefaultNitermin():
    algo = Algorithm(type="Basic_Scheme", convergenceAcceleration=True)
    algo.nitermin = 0
    algo.niterminACV = 0
    param_algo = AlgorithmParameters(
        algorithm=algo,
        mechanics=Mechanics(filter="Default", smallPerturbations=True),
    )
    assert compareXMLWithRef(param_algo, "algo_default_Nitermin.xml")


def test_XML_AlgoDefaultSubstep():
    sub = Substepping()
    sub.nitermax2 = 100
    sub.geomRatio = 1.0
    sub.nsub = 10
    sub.depth = 3
    algo = Algorithm(
        type="Basic_Scheme",
        convergenceAcceleration=True,
        nitermax=1000,
        convergenceCriterion=1e-4,
    )
    algo.substepping = sub
    param_algo = AlgorithmParameters(
        algorithm=algo,
        mechanics=Mechanics(filter="Default", smallPerturbations=False),
    )
    assert compareXMLWithRef(param_algo, "algo_default_substep.xml")


def test_XML_AlgoDefaultLaminate():
    algoLaminate = AlgorithmLaminate()
    algoLaminate.convergenceAcceleration = True
    algoLaminate.initializationType = "Linear"
    algoLaminate.nIncrements = 1
    algoLaminate.nMaxSubdivision = 5
    param_algo = AlgorithmParameters(
        algorithm=Algorithm(type="Basic_Scheme", convergenceAcceleration=True),
        mechanics=Mechanics(filter="Default", smallPerturbations=True),
    )
    param_algo.algorithmLaminate = algoLaminate
    assert compareXMLWithRef(param_algo, "algo_default_voxcomplaminate.xml")


def test_XML_AlgoOcta():
    param_algo = AlgorithmParameters(
        algorithm=Algorithm(type="Basic_Scheme", convergenceAcceleration=True),
        mechanics=Mechanics(filter="octa", smallPerturbations=True),
    )
    assert compareXMLWithRef(param_algo, "algo_octa.xml")


def test_XML_AlgoNoFilter():
    param_algo = AlgorithmParameters(
        algorithm=Algorithm(
            type="Basic_Scheme",
            convergenceAcceleration=False,
            convergenceCriterion=1e-5,
        ),
        mechanics=Mechanics(filter="Default", smallPerturbations=True),
    )
    param_algo.mechanics.filter.type = "no_filter"
    assert compareXMLWithRef(param_algo, "algo_old.xml")


def test_XML_AlgoSmacroGD():
    param_algo = AlgorithmParameters(
        algorithm=Algorithm(type="Basic_Scheme", convergenceAcceleration=True),
        mechanics=Mechanics(filter="default", smallPerturbations=False),
    )
    param_algo.algorithm.convergenceCriterionSmacro = 1.0e-3
    assert compareXMLWithRef(param_algo, "algo_Smacro_GD.xml")


def test_XML_CharTraction1():
    lo = LoadingOutput()

    output = Output()
    output.setVtkStressStrain(stress=1, strain=1)
    lo.output = output

    loading = Loading()
    loading.setTimeDiscretizationLinear(1, 100.0)
    loading.setLinearEvolution((0, 0), MechanicDriving.Strain, 0.01)
    for i in range(0, 3):
        for j in range(i, 3):
            if i != 0 or j != 0:
                loading.setLinearEvolution((i, j), MechanicDriving.Stress, 0.0)
    lo.add(loading)
    assert compareXMLWithRef(lo, "char_traction_1.xml")


def test_XML_CycleTraction():
    output = Output()
    output.setVtkStressStrain(stress=1, strain=1)
    output.addZone(0, [0])

    def commonLoading():
        loading0 = Loading()
        loading0.setOutputVtkList([1])
        for i in range(0, 3):
            for j in range(i, 3):
                if i != 0 or j != 0:
                    loading0.setLinearEvolution((i, j), MechanicDriving.Stress, 0.0)
        return loading0

    loading0 = commonLoading()
    loading0.setTimeDiscretizationLinear(50, 50)
    loading0.setLinearEvolution((0, 0), MechanicDriving.Strain, 0.005)
    loading0.setOutputCell(10)
    loading1 = commonLoading()
    loading1.setTimeDiscretizationLinear(100, 150)
    loading1.setLinearEvolution((0, 0), MechanicDriving.Strain, -0.005)
    loading1.setOutputCell(20)
    loading2 = commonLoading()
    loading2.setTimeDiscretizationLinear(100, 250)
    loading2.setLinearEvolution((0, 0), MechanicDriving.Strain, 0.005)

    lo = LoadingOutput()
    lo.output = output
    lo.add(loading0)
    lo.add(loading1)
    lo.add(loading2)
    assert compareXMLWithRef(lo, "char_cycle_traction.xml")


def test_XML_CharDefImp():
    lo = LoadingOutput()

    output = Output()
    output.vtkStressStrain = VtkStressStrain(stress=1, strain=1)
    lo.output = output

    loading = Loading()
    loading.setTimeDiscretizationUser([32832.0])
    for i in range(0, 3):
        for j in range(i, 3):
            if i != j:
                loading.setLinearEvolution((i, j), MechanicDriving.Strain, 0.0)
    loading.setLinearEvolution(Component.XX, MechanicDriving.Strain, -5e-4)
    loading.setLinearEvolution(Component.YY, MechanicDriving.Strain, 1.5e-4)
    loading.setLinearEvolution(Component.ZZ, MechanicDriving.Strain, 1.5e-4)
    loading.setOutputVtkList([1])
    lo.add(loading)

    assert compareXMLWithRef(lo, "char_def_imp.xml")


def test_XML_CharCycleTraction():
    lo = LoadingOutput()

    output = Output()
    output.vtkStressStrain = VtkStressStrain(1, 1)
    output.addZone(0, [0])
    lo.output = output

    for l in range(3):
        loading = Loading()
        loading.setTimeDiscretizationLinear(50 if l == 0 else 100, 100.0 * l + 50.0)
        loading.setOutputVtkList([1])
        if l < 2:
            loading.setOutputCell(10 * (l + 1))
        loading.setLinearEvolution(
            Component.XX, MechanicDriving.Strain, 5e-3 if l % 2 == 0 else -5e-3
        )
        for i in range(3):
            for j in range(i, 3):
                if i != 0 or j != 0:
                    loading.setLinearEvolution((i, j), MechanicDriving.Stress, 0.0)
        lo.add(loading)

    assert compareXMLWithRef(lo, "char_cycle_traction.xml")


def prepareTriax2p5(ds: DirStress):
    lo = LoadingOutput()
    output = Output()
    output.vtkStressStrain = VtkStressStrain(0, 0)
    lo.output = output
    loading = Loading()
    loading.setTimeDiscretizationLinear(40, 40.0)
    loading.setOutputVtkList([1])
    loading.setConstantEvolution(Component.XX, MechanicDriving.Stress, 1.0)
    loading.setConstantEvolution(Component.YY, MechanicDriving.Stress, 1.3)
    loading.setLinearEvolution(Component.ZZ, MechanicDriving.Strain, 4e-3, 1.6)
    for i in range(3):
        for j in range(3):
            if i != j:
                loading.setConstantEvolution((i, j), MechanicDriving.Stress, 0.0)

    loading.setDirStress(ds)
    lo.add(loading)
    return lo


def test_XML_CharTriax2p5PK1():
    lo = prepareTriax2p5(DirStress.PK1)
    assert compareXMLWithRef(lo, "char_triax2p5_PK1.xml")


def test_XML_CharTriax2p5Cauchy():
    lo = prepareTriax2p5(DirStress.Cauchy)
    assert compareXMLWithRef(lo, "char_triax2p5_cauchy.xml")


def test_XML_CharDefImpFlexionBeam():
    lo = LoadingOutput()

    output = Output()
    output.vtkStressStrain = VtkStressStrain(stress=1, strain=1)
    lo.output = output

    loading = Loading()
    loading.setTimeDiscretizationLinear(1, 100.0)
    for i in range(0, 3):
        for j in range(i, 3):
            if i != 0 or j != 0:
                loading.setLinearEvolution((i, j), MechanicDriving.Strain, 0.0)
    loading.setLinearEvolution(Component.XX, MechanicDriving.Strain, 1e-2)
    loading.setGradGradU(Component.XX, Component.Y, Evolution.Linear, 1e-3)
    loading.setGradGradU(Component.YY, Component.Y, Evolution.Linear, -3e-4)
    loading.setGradGradU(Component.ZZ, Component.Y, Evolution.Linear, -3e-4)
    loading.setOutputVtkList([1])
    lo.add(loading)

    assert compareXMLWithRef(lo, "char_defimp_flexion_beam.xml")


def test_XML_CharDefImpTorsionBeam():
    lo = LoadingOutput()

    output = Output()
    output.vtkStressStrain = VtkStressStrain(stress=1, strain=1)
    lo.output = output

    loading = Loading()
    loading.setTimeDiscretizationLinear(1, 100.0)
    for i in range(0, 3):
        for j in range(i, 3):
            if i != 0 or j != 0:
                loading.setLinearEvolution((i, j), MechanicDriving.Strain, 0.0)
    loading.setLinearEvolution(Component.XX, MechanicDriving.Strain, 1e-2)
    loading.setGradGradU(Component.ZX, Component.Y, Evolution.Linear, 2e-3)
    loading.setGradGradU(Component.YX, Component.Z, Evolution.Linear, -2e-3)
    loading.setOutputVtkList([1])
    lo.add(loading)

    assert compareXMLWithRef(lo, "char_defimp_torsion_beam.xml")


def test_XML_CharTractionGD():
    lo = LoadingOutput()

    output = Output()
    output.vtkStressStrain = VtkStressStrain(1, 1)
    lo.output = output

    loading = Loading()
    loading.setTimeDiscretizationLinear(500, 500.0)
    loading.setLinearEvolution(Component.XX, MechanicDriving.Stress, 0.0)
    loading.setLinearEvolution(Component.YY, MechanicDriving.Stress, 0.0)
    loading.setLinearEvolution(Component.ZZ, MechanicDriving.Strain, 0.05)
    loading.setLinearEvolution(Component.XY, MechanicDriving.Stress, 0.0)
    loading.setLinearEvolution(Component.XZ, MechanicDriving.Stress, 0.0)
    loading.setLinearEvolution(Component.YZ, MechanicDriving.Stress, 0.0)
    loading.setLinearEvolution(Component.YX, MechanicDriving.Strain, 0.0)
    loading.setLinearEvolution(Component.ZX, MechanicDriving.Strain, 0.0)
    loading.setLinearEvolution(Component.ZY, MechanicDriving.Strain, 0.0)
    lo.add(loading)

    assert compareXMLWithRef(lo, "char_traction_GD.xml")


def test_XML_CharTractionPolyXGD():
    lo = LoadingOutput()

    output = Output()
    output.vtkStressStrain = VtkStressStrain(1, 1)
    lo.output = output

    loading = Loading()
    loading.setTimeDiscretizationLinear(2000, 2000.0)
    loading.setOutputVtkList([2000])
    loading.setLinearEvolution(Component.XX, MechanicDriving.Strain, 0.2)
    for i in range(3):
        for j in range(i, 3):
            if i != 0 or j != 0:
                loading.setConstantEvolution((i, j), MechanicDriving.Stress)
    for j in range(0, 3):
        for i in range(j + 1, 3):
            loading.setConstantEvolution((i, j), MechanicDriving.Strain)
    lo.add(loading)

    assert compareXMLWithRef(lo, "char_traction_polyx_GD.xml")


def test_XML_CharThermoBicouche():
    lo = LoadingOutput()

    output = Output()
    output.vtkStressStrain = VtkStressStrain(1, 1)
    lo.output = output

    initload = InitLoadExt()
    initload.temperature = 0.01
    lo.initLoadExt = initload

    loading = Loading()
    loading.setTimeDiscretizationLinear(10, 100.0)
    for i in range(3):
        for j in range(i, 3):
            loading.setLinearEvolution((i, j), MechanicDriving.Strain, 0)
    loading.setTemperatureEvolution(Evolution.Constant)
    lo.add(loading)

    assert compareXMLWithRef(lo, "char_thermo_bicouche.xml")


def test_XML_CharParamExtBicouche():
    lo = LoadingOutput()

    output = Output()
    output.vtkStressStrain = VtkStressStrain(1, 1)
    lo.output = output

    initload = InitLoadExt()
    initload.setParam(0, 0.01)
    lo.initLoadExt = initload

    loading = Loading()
    loading.setTimeDiscretizationLinear(10, 100.0)
    for i in range(3):
        for j in range(i, 3):
            loading.setLinearEvolution((i, j), MechanicDriving.Strain, 0)
    loading.setParamEvolution(0, Evolution.Constant)
    lo.add(loading)

    assert compareXMLWithRef(lo, "char_paramext_bicouche.xml")


def test_XML_CharDiffusionFlux():
    loadings = LoadingOutput()
    loadings.output.setVtkFluxDGradD(1, 1)
    loadings.output.addZone(0)
    load = Loading()
    load.setTimeDiscretizationUser([1.0])
    load.setOutputZone(1)
    load.setOutputVtkList([1])
    for i in range(0, 3):
        load.setEvolution(i, DiffusionDriving.Flux, Evolution.Linear, float(i + 1))
    loadings.add(load)
    assert compareXMLWithRef(loadings, "char_diffusion_flux.xml")


TIME_LIST_0 = [
    83808,
    143424,
    211680,
    289440,
    380160,
    483840,
    604800,
    738720,
    898560,
    1080000,
    1287360,
    1529280,
    1801440,
    2116800,
    2479680,
    2903040,
    3386880,
    3939840,
    4579200,
    5313600,
    6160320,
    7128000,
    8251200,
    9538560,
    11016000,
    12718080,
    14670720,
    16934400,
    19526400,
    22464000,
    25920000,
]


def test_XML_CharFluage():
    lo = LoadingOutput()

    output = Output()
    output.vtkStressStrain = VtkStressStrain(stress=1, strain=1)
    output.addZone(1)
    lo.output = output

    loading = Loading()
    loading.setTimeDiscretizationUser([32832])
    loading.setOutputZone(1)
    for i in range(0, 3):
        for j in range(i, 3):
            if i != 0 or j != 0:
                loading.setLinearEvolution((i, j), MechanicDriving.Stress, 0.0)
    loading.setLinearEvolution(Component.XX, MechanicDriving.Strain, 1e-2)
    lo.add(loading)

    loading1 = Loading()
    loading1.setTimeDiscretizationUser(TIME_LIST_0)
    loading1.setOutputVtkList([31])
    loading1.setOutputZone(30)
    for i in range(0, 3):
        for j in range(i, 3):
            loading1.setConstantEvolution((i, j), MechanicDriving.Stress)
    lo.add(loading1)
    assert compareXMLWithRef(lo, "char_fluage.xml")


def test_XML_CharFluage2():
    lo = LoadingOutput()

    output = Output()
    output.vtkStressStrain = VtkStressStrain(stress=1, strain=1)
    output.addZone(1)
    lo.output = output

    loading = Loading()
    loading.setTimeDiscretizationUser([32832])
    loading.setOutputZone(1)
    for i in range(0, 3):
        for j in range(i, 3):
            if i != 0 or j != 0:
                loading.setLinearEvolution((i, j), MechanicDriving.Stress, 0.0)
    loading.setLinearEvolution(Component.XX, MechanicDriving.Strain, 1.2e-2)
    loading.addUserInterruptValue(0.42706742e09)
    lo.add(loading)

    loading1 = Loading()
    loading1.setTimeDiscretizationUser(TIME_LIST_0)
    loading1.setOutputVtkList([31])
    loading1.setOutputZone(30)
    for i in range(0, 3):
        for j in range(i, 3):
            loading1.setConstantEvolution((i, j), MechanicDriving.Stress)
    lo.add(loading1)
    assert compareXMLWithRef(lo, "char_fluage2.xml")


def test_XML_CharOutputFullOptions():
    lo = LoadingOutput()

    output = Output()
    output.vtkStressStrain = VtkStressStrain(1, 0)
    output.addZone(0, [0])
    output.addZone(2)
    output.addVtkIntVarList(0, [2])
    output.addVtkIntVarList(1, [0, 1])
    lo.output = output

    loading = Loading()
    loading.setTimeDiscretizationLinear(1000, 10.0)
    loading.setOutputCell(100)
    loading.setOutputZone(10)
    loading.setOutputVtkList((500, 1000))
    loading.setLinearEvolution(Component.XX, MechanicDriving.Strain, 0.01)
    for i in range(3):
        for j in range(i, 3):
            if i != 0 or j != 0:
                loading.setLinearEvolution((i, j), MechanicDriving.Stress, 0.0)
    lo.add(loading)
    assert compareXMLWithRef(lo, "char_output_full_options.xml")


def test_XML_MatBeton():
    materials = Materials()

    ref = ReferenceMaterial(lambda0=2.0952e10, mu0=1.5014e10)
    materials.referenceMaterial = ref

    mat = Material()
    mat.setLaw("viscoelas_maxwell")
    mat.setCoeffs(
        [
            70e9,
            0.3,
            4,
            3.58873e9,
            3.10474e9,
            6.4781e9,
            3.0942e9,
            172800,
            7.51552e9,
            3.15347e9,
            1728000,
            5.10283e9,
            3.15347e9,
            17280000,
        ]
    )
    mat.setCoeffName(10, "NameCoeffbidon11")

    for i in range(36):
        mat.addIntVar(0.0)
    mat.setIntVarName(2, "NameVIbidon3")
    materials.add(mat)

    mat2 = Material()
    mat2.setLaw("elasiso")
    mat2.setCoeffs([4.0385e10, 2.6923e10])
    materials.add(mat2)

    assert compareXMLWithRef(materials, "mat_beton.xml")


def test_XML_ThermMerope0():
    algo = Algorithm.createDefault()
    algo.type = "Basic_Scheme"
    algo.convergenceAcceleration = True
    algo.nitermax = 3000
    param_algo = AlgorithmParameters(Algorithm.createDefault())
    param_algo.algorithm = algo
    diffu = Diffusion.createDefault()
    diffu.filter = "Default"
    diffu.stationary = True
    param_algo.diffusion = diffu

    assert compareXMLWithRef(param_algo, "algorithm-0.xml")

    materials = Materials()
    materials.referenceMaterialD = ReferenceMaterialD(214.8)

    kappas = [0.6, 429.0]
    for i in range(0, 2):
        mat = Material()
        mat.lawK = "Fourier_iso_polarization"
        mat.coeffKs = [kappas[i], 0.0, 0.0, -kappas[i]]
        materials.add(mat)

    assert compareXMLWithRef(materials, "material-0.xml")

    loadings = LoadingOutput()
    loadings.output.vtkFluxDGradD = VtkFluxDGradD(0, 0)
    load = Loading()
    load.setTimeDiscretizationUser([1.0, 2.0])
    for i in range(0, 3):
        load.setEvolution(i, DiffusionDriving.Gradient, Evolution.Linear, 0.0)
    loadings.add(load)
    assert compareXMLWithRef(loadings, "loading-0.xml")
