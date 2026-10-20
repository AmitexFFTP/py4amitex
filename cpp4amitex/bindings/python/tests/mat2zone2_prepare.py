from py4amitex.input import (
    Input,
    Grid,
    AlgorithmParameters,
    Algorithm,
    Diffusion,
    LoadingOutput,
    Loading,
    ReferenceMaterialD,
    Materials,
    Material,
    Component,
    DiffusionDriving,
    Zone,
    InitLoadExt,
    Evolution,
)


def mat2zone2Prepare(nbMat: int) -> None:
    assert 2 >= nbMat >= 1
    grid = Grid([32, 32, 32], [3125, 3125, 3125])
    kappas = [0.6, 429.0]
    input = Input(
        grid,
        AlgorithmParameters(Algorithm.createDefault()),
        Materials(),
        LoadingOutput(),
    )
    input.grid = grid
    input.resultsDir = f"testresults/addzoneandlaunchthermal_{nbMat}"

    diffu = Diffusion.createDefault()
    diffu.filter = "Default"
    diffu.stationary = True
    input.algorithmParameters.diffusion = diffu
    input.algorithmParameters.algorithm.convergenceAcceleration = True
    input.algorithmParameters.algorithm.type = "Basic_Scheme"

    init = InitLoadExt()
    init.temperature = 300
    init.setParam(0, 0)
    input.loadingOutput.initLoadExt = init

    loading = Loading()
    loading.setTimeDiscretizationLinear(1, 1.0)
    loading.setLinearEvolution(Component.X, DiffusionDriving.Gradient, 0.0)
    loading.setLinearEvolution(Component.Y, DiffusionDriving.Gradient, 0.0)
    loading.setLinearEvolution(Component.Z, DiffusionDriving.Gradient, 0.0)
    loading.setTemperatureEvolution(Evolution.Constant)
    loading.setParamEvolution(0, Evolution.Linear, 1)
    input.loadingOutput.add(loading)

    materials = Materials()
    materials.referenceMaterialD = ReferenceMaterialD(214.8)

    NX, NY, NZ = grid.dims()
    DX, DY, DZ = grid.voxelLengths()
    R = 0.3 * DX * NX
    center = [NX * DX / 2, NX * DY / 2, NX * DZ / 2]
    zone0 = Zone(grid.dims())
    zone1 = Zone(grid.dims())
    for point in grid.allPoints():
        if grid.distance(point, center) < R:
            zone1.add(point)
        else:
            zone0.add(point)

    for m in range(nbMat):
        mat = Material()
        mat.setLawK("Fourier_iso_polarization")
        mat.setNumberCoeffK(4)
        if nbMat == 1:
            mat.addZone(zone0, [], [kappas[0], 0.0, 0.0, -kappas[0]])
            mat.addZone(zone1, [], [kappas[1], 0.0, 0.0, -kappas[1]])
        elif m == 0:
            mat.addZone(zone0, [], [kappas[0], 0.0, 0.0, -kappas[0]])
        else:
            mat.addZone(zone1, [], [kappas[1], 0.0, 0.0, -kappas[1]])
        materials.add(mat)
    input.materials = materials
    return input
