from warnings import warn
from itertools import product

import pytest

from py4amitex.input.field import FieldInt, FieldDouble

from .utils import testsdir


def test_field():
    field = FieldInt([5, 6, 2])
    field[0, 3, 1] = 4
    assert field.at(0, 3, 1) == 4
    assert field.lbound(0) == 0
    assert field.lbound(1) == 0
    assert field.lbound(2) == 0
    assert field.ubound(0) == 5
    assert field.ubound(1) == 6
    assert field.ubound(2) == 2
    assert field.dims() == [5, 6, 2]

    assert field.inBounds([1, 2, 0])
    assert field.inBounds([4, 5, 1])
    assert field.inBounds([2, 4, 0])
    assert field.inBounds(2, 0, 1)
    assert not field.inBounds([2, 0xFFFFFFFFFFFFFFFF, 1])
    assert not field.inBounds([0, 0, 2])
    assert not field.inBounds([6, 5, 1])
    assert not field.inBounds(4, 6, 0)

    field.fill(6)

    for i in range(5):
        for j in range(6):
            for k in range(2):
                assert field[i, j, k]
    with pytest.raises(IndexError):
        print(field[0, 3, 2])


def test_field_np():
    f = FieldDouble([7, 6, 2])
    try:
        import numpy as np

        farr = np.zeros((7, 6, 2), order="F")
        farr[6, 5, 1] = 42.0

        f = FieldDouble(farr)
        assert f[6, 5, 1] == 42.0

        farr[5, 5, 0] = 33.0
        assert f[5, 5, 0] == 33.0

        farr2 = np.zeros((7, 6, 2))
        farr2[3, 2, 1] = 24.0
        f = FieldDouble(farr2, copy=True)
        assert f[3, 2, 1] == 24.0
    except ModuleNotFoundError:
        warn(UserWarning("Numpy module not found"))


def test_field_from_file():
    f = FieldDouble.loadFromVtk(f"{testsdir}/ref-amxdir/elaiso_eigs/intvar_1_1.vtk")
    assert f.dims() == [32, 32, 32]
    for i in product(range(32), repeat=3):
        assert f[i[0], i[1], i[2]] == 0.0
