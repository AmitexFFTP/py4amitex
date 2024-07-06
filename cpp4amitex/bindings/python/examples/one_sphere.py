from py4amitex.input import (
    Input,
    Grid,
    Algorithm,
    Diffusion,
    Material,
    Zone,
    ReferenceMaterialD,
    Loading,
    DiffusionDriving,
    Evolution,
    LoadingOutput,
    AlgorithmParameters,
    Materials,
)
from py4amitex.simulation import runSimulationExternal


def prepareInput():
    DL = 0.0003125
    NV = 32
    R = 0.3 * DL * NV
    center = [NV // 2, NV // 2, NV // 2]
    kappas = [0.6, 429.0]

    input = Input()
    grid = Grid([NV, NV, NV], [DL, DL, DL])
    input.grid = grid

    materials = Materials()

    materials.referenceMaterialD = ReferenceMaterialD(214.8)

    zone0, zone1 = Zone(grid.dims()), Zone(grid.dims())
    zone0.add(p for p in grid.allPoints() if grid.distance(p, center) >= R)
    zone1.add(p for p in grid.allPoints() if grid.distance(p, center) < R)

    material = Material()
    material.setLawK("Fourier_iso_polarization")
    material.setNumberCoeffK(4)

    material.addZone(zone0, [], [kappas[0], 0.0, 0.0, -kappas[0]])
    material.addZone(zone1, [], [kappas[1], 0.0, 0.0, -kappas[1]])

    materials.add(material)

    algorithm = Algorithm(type="Basic_Scheme", convergenceAcceleration=True)
    algorithm.convergenceCriterion = 1.0e-4
    algorithm.nitermax = 3000

    diffusion = Diffusion(filter="Default", stationary=True)
    algorithmParameters = AlgorithmParameters(algorithm, diffusion=diffusion)

    loadingOutput = LoadingOutput()
    loadingOutput.output.setVtkFluxDGradD(1, 1)

    loading = Loading()
    loading.setTimeDiscretizationLinear(1, 1.0)
    for i in range(3):
        loading.setEvolution(i, DiffusionDriving.Gradient, Evolution.Linear, 0.0)
    loading.setOutputVtkList([1])
    loadingOutput.add(loading)

    input = Input(grid, algorithmParameters, materials, loadingOutput)
    input.resultsDir = "amitex_dir_one_sphere"

    return input


def extractFluxGrad(prefix):
    flux = []
    grad = []

    def _toFloat(s: str) -> float:
        from math import nan

        ret = nan
        try:
            ret = float(s)
        except ValueError:
            # numbers like 0.49801817-310 are invalid, why Fortran outputs them ?
            pass
        return ret

    with open(prefix + ".std", encoding="utf-8") as file:
        for line in file:
            if line and line[0] == "#":
                continue
            vals = line.split()
            flux.append([float(tmp) for tmp in vals[1:4]])
            grad.append([float(tmp) for tmp in vals[4 : 4 + 3]])
    return flux, grad


if __name__ == "__main__":
    input = prepareInput()
    runSimulationExternal(input)
    print("Flux\tGradient")
    flux, grad = extractFluxGrad(input.outputPrefix())
    for fX, gX in zip(flux[-1], grad[-1]):
        print(f"{fX:16.7e}\t{gX:16.7e}")
