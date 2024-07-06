import numpy as np
from numpy.random import default_rng
from py4amitex.input import Grid, Material
from py4amitex.input.field import FieldDouble

grid = Grid([3, 3, 3], [1., 1., 1.])

intVar0Init = np.empty(grid.dims(), order='F')
default_rng().random(out=intVar0Init)

material = Material()
material.addIntVar(FieldDouble(intVar0Init))