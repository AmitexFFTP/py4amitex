#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Unit test module for Grid class.

@author: amarano
"""

import unittest
import numpy as np
from py4amitex.microgen.geometry import Grid

class TestInitGrid(unittest.TestCase):
    """Unit tests for Grid class initialization."""

    def test_default(self):
        """Test default initialization of grid object."""
        # create a grid object
        g = Grid()
        # test repr method
        s = str(Grid)
        # test values
        self.assertEqual(g.dimension, 2)
        self.assertEqual(len(g.L), 2)
        self.assertEqual(g.L[0], 1.)
        self.assertEqual(g.N[0], 100)
        self.assertEqual(g.dx[1], (g.L[1] / g.N[1]))

    def test_2D_grid(self):
        """Test set up of a 2D grid object."""
        # create object
        g = Grid(dimension=2, resolution=(50,100), size=(1.,2.),
                 origin=(0.5,-2.0))
        # test values
        self.assertEqual(g.dimension, 2)
        self.assertEqual(g.O[0], 0.5)
        self.assertEqual(g.L[0], 1.)
        self.assertEqual(g.L[1], 2.)
        self.assertEqual(g.N[0], 50)
        self.assertEqual(g.dx[1], 0.02)
        # test coordinates
        self.assertAlmostEqual(np.max(g.VY[:]), 0.)
        self.assertAlmostEqual(np.min(g.CX[:]), 0.5 + 0.5*(1./50.))

    def test_3D_grid(self):
        """Test set up of a 2D grid object."""
        # create object
        g = Grid(dimension=3, resolution=(50,100,200), size=(1.,2.,3.),
                 origin=(0.5,-2.0,3.0))
        # test values
        self.assertEqual(g.dimension, 3)
        self.assertEqual(g.O[2], 3.0)
        self.assertEqual(g.L[2], 3.0)
        self.assertEqual(g.L[1], 2.)
        self.assertEqual(g.N[2], 200)
        self.assertEqual(g.dx[2], (3.0/200.))
        # test coordinates
        self.assertEqual(g.VZ.shape[2], g.N[2]+1)
        self.assertAlmostEqual(np.min(g.VZ[:]), g.O[2])
        self.assertAlmostEqual(np.max(g.VZ[:]), g.O[2] + g.L[2])
        self.assertEqual(g.CX.shape[2], g.N[2])
        self.assertAlmostEqual(np.min(g.CZ[:]), g.O[2] + 0.5*g.dx[2])
        self.assertAlmostEqual(np.max(g.CZ[:]), g.O[2] + g.L[2] - 0.5*g.dx[2])