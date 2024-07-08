Fields
======


Implements data types:

-  64-bit floating point: FieldDouble 
-  integer: FieldInt

Basically a 3D array. Useful for e.g. internal variables.

The array-like functionnality is limited, but you can convert a numpy Array
to it (with an array created with order="F" you can avoid a copy of the underlying memory, otherwise use
the constructor with ``copy=True``)

.. code-block:: python

    import numpy as np
    from amitex.field import FieldDouble

    npArr0 = np.zeros((10, 10, 10), order="F")
    field = FieldDouble(npArr0)

    npArr1 = np.zeros((10, 10, 10), order="C")
    field = FieldDouble(npArr1, copy=True)

Alternatively, you can make a numpy array pointing to the underlying
memory buffer.

.. code-block:: python

    field = FieldDouble((10,10,10))
    field.fill(4)
    fieldArray = np.array(field, copy = False)
    dx = 1.e-3
    points = np.mgrid[0:10, 0:10, 0:10]
    fieldArray[:,:,:] = points / 2
    assert field[1,1,1] == 0.5

Fields are primarely useful for initialiazing internal variable

.. code-block:: python

    import numpy as np
    from numpy.random import default_rng
    from amitex.input import Grid, Material
    from amitex.input.field import FieldDouble

    grid = Grid([3, 3, 3], [1., 1., 1.])

    intVar0Init = np.empty(grid.dims(), order='F')
    default_rng().random(out=intVar0Init)

    material = Material()
    material.addIntVar(FieldDouble(intVar0Init))

.. autoclass:: py4amitex.input.field.FieldDouble
    :members:

.. autoclass:: py4amitex.input.field.FieldInt
    :members: