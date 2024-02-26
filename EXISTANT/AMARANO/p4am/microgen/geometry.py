#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
P4A Module to implement shape generators to construct microstructures.

TODO:
    -- complete docstrings

CONVENTIONS:
    * This class implements geometry algorithms that create 2D or 3D images

    * The origin of geometrical grids is associated to the first corner of the
      [0,0] or [0,0,0] pixel/voxel.

    * The default grid squared/cubic and has 100 elements along one dimension.
      To change grid dimensions, specific options must be provided

@author: amarano
"""

import numpy as np

class ShapeGenerator:
    def __init__(self):
        """Class to generate geometrical objects in 2D or 3D images."""
        pass

    @staticmethod
    def circle(radius=1., center=(0.,0.), resolution=100, grid=None):
        """Creates a binary 3D image of a circle.

        Parameters
        ----------
        radius : float, optional
            Radius of the circle. Default is 1.

        center : tuple(Cx, Cy) floats, optional
            Coordinates of the center of the circle. Default is (0.,0.).

        resolution : int, optional
            Number of pixels to use for the resolution of the 2D image.
            Default is 100. If a grid argument is passed, this
            argument is ignored.

        grid : Grid object, optional
            grid object to use to create the circle. If None is provided,
            a grid is created with default parameters, for the region
            (-1.0:1.0, -1.0:1.0)

        Returns
        -------
        center : np.array()
            Two dimensional binary image of the circle.
        """
        # if no grid, create one
        if grid is None:
            grid = Grid(origin=(-radius,-radius), size=(2*radius,2*radius),
                        resolution=resolution)
        # check bidimensional grid
        if grid.dimension != 2:
            raise ValueError("Circle creation is possible only with 2D grids.")
        # Compute distance to circle center
        distance = np.sqrt((grid.CX-center[0])**2 + (grid.CY-center[1])**2)
        # create binary image
        circle = (distance <= radius)
        return circle

class Grid:
    """Class to handle 2D or 3D grids."""

    def __init__(self, dimension=2, resolution=100, size=1., origin=(0.,0.)):
        """Create a regular grid object.


        Parameters
        ----------
        dimension : int, optional
            Number of dimensions of the grid. The default is 2.

        resolution : int, optional
            Number of elements of the grid along each dimension. Can be an
            array of a scalar. The default is 100.
            Array : sets resolution along each dimension (ex N = [Nx, Ny])
            Scalar : sets resolution for all dimensions (ex N = [N, N, N])

        size : float, optional
            Size of the grid along each dimension. Can be an array of a scalar.
            The default is 1.
            Array : sets size along each dimension (ex L = [Lx, Ly])
            Scalar : sets resolution for all dimensions (ex L = [L, L, L])

        origin : tuple(Ox, Oy, Oz), optional
            Coordinates of the first corner of the (0,0) or (0,0,0) element.
            Size must match grid dimension. Defaut value is (0.,0.).

        """
        # In the following unit cell described by three vectors :
        #     L -> unit cell size, D --> unit cell voxel size,
        #     R -> unit cell resolution
        self.dimension = dimension
        # Initialize grid size
        self._init_size(size)
        # Initialize grid resolution
        self._init_resolution(resolution)
        # Initialize grid voxel size (spacing)
        self._init_spacing()
        # Initialize grid origin
        self._init_origin(origin)
        # Create coordinates grid
        self._init_coordinates()

    def __repr__(self):
        """String representation of Grid object."""
        s = f"\nRegular Grid Object -- Dimension : {self.dimension}"
        s += f"\n\t * origin {self.O}"
        s += f"\n\t * size {self.L}"
        s += f"\n\t * resolution {self.N}"
        s += f"\n\t * spacing {self.dx}"
        return s

    def _init_size(self, size):
        """Initialize grid object size."""
        # check case (scalar or array input value)
        if np.isscalar(size):
            self.L = size*np.ones((self.dimension,), dtype=np.float32)
        elif len(size) == self.dimension:
            self.L = np.array(size, dtype=np.float32)
        else:
            raise ValueError("Grid size must be provided as a scalar value, "
                             "or an array with a length equal to the grid "
                             "dimension.")
        # check zero values
        if any(self.L == 0):
            raise ValueError("Grid size cannot be zero along any direction.")

    def _init_resolution(self, resolution):
        """Initialize grid object resolution."""
        # Check case (scalar or array input value)
        if np.isscalar(resolution):
            self.N = resolution*np.ones((self.dimension,), dtype=np.int32)
        elif len(resolution) == self.dimension:
            self.N = np.array(resolution, dtype=np.int32)
        else:
            raise ValueError("Grid resolution must be provided as a scalar "
                             "value, or an array with a length equal to the "
                             "grid dimension.")
        # Check zero values
        if any(self.N == 0):
            raise ValueError("Grid resolution cannot be zero along any "
                             "direction.")

    def _init_spacing(self):
        """Initialize grid voxel size."""
        self.dx = np.zeros((self.dimension,), dtype=np.float64)
        for i in range(self.dimension):
            self.dx[i] = self.L[i] / self.N[i]

    def _init_origin(self, origin):
        """Initialize grid origin."""
        # check origin size
        if len(origin) != self.dimension:
            raise ValueError("Grid origin must have the same dimension as the"
                             f" grid dimension : {self.dimension}.")
        # initialize origin
        self.O = np.array(origin, dtype=np.float64)

    def _init_coordinates(self):
        """Initialize grid coordinate grid."""
        # create coordinate range for grid vertexes
        X = np.arange(self.O[0], self.O[0] + self.L[0] + self.dx[0],
                      self.dx[0])
        # create Y coordinate range
        Y = np.arange(self.O[1], self.O[1] + self.L[1] + self.dx[1],
                      self.dx[1])
        # create coordinate range for grid centers
        XC = np.arange(self.O[0] + 0.5*self.dx[0],
                       self.O[0] + self.L[0],
                       self.dx[0])
        # create Y coordinate range
        YC = np.arange(self.O[1] + 0.5*self.dx[1],
                       self.O[1] + self.L[1],
                       self.dx[1])
        # create Z coordinate range if needed
        if self.dimension == 3:
            # grid vertexes
            Z = np.arange(self.O[2], self.O[2] + self.L[2] + self.dx[2],
                          self.dx[2])
            # grid centers
            ZC = np.arange(self.O[2] + 0.5*self.dx[2],
                           self.O[2] + self.L[2],
                          self.dx[2])
        # create grid of coordinates
        if self.dimension == 2:
            XX, YY = np.meshgrid(X, Y)
            XXC, YYC = np.meshgrid(XC, YC)
        elif self.dimension == 3:
            XX, YY, ZZ = np.meshgrid(X, Y, Z)
            XXC, YYC, ZZC = np.meshgrid(XC, YC, ZC)
        else:
            raise ValueError("Dimension of the grid must be 2 or 3.")
        # assing vertexes coordinates to grid object
        self.VX = XX
        self.VY = YY
        if self.dimension == 3:
            self.VZ = ZZ
        # assing centers coordinates to grid object
        self.CX = XXC
        self.CY = YYC
        if self.dimension == 3:
            self.CZ = ZZC



