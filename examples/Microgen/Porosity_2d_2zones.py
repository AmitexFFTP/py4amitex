#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Script to create a 2D unit cell with an homogeneous material with a circular
porosity.
A boundary layer in the material around the porosity is considered as a
specific zone.

Two materials :
    - Id 1 : solid material
    - Id 2 : cavity (no material)

@author: amarano
"""
#%% Imports

import numpy as np

from matplotlib import pyplot as plt
from pathlib import Path

from py4amitex.microgen.geometry import Grid, ShapeGenerator
from py4amitex.microgen.amitex_geometry import AGeom

from py4amitex.microgen.unit_cell_tools import UnitCellTools as UCT

#%% Pathes

VtkDir = Path.cwd() / "Porosity"

#%% Parameters

UnitCell_Size = 1.
Resolution = 3000

porosity_ratio = 0.016

Porosity_Radius = np.sqrt(porosity_ratio / np.pi)*UnitCell_Size
Porosity_Center = np.array([0.5,0.5])*UnitCell_Size

#%% Set vtk files basename

filename = f"Porosity_res{Resolution}_f{porosity_ratio*100:.1f}"
filename = filename.replace('.', 'p')
File_basename = VtkDir / filename

#%% Unit cell creation

g = Grid(resolution=Resolution, size=UnitCell_Size)

# create a binary image of a circle
circle = ShapeGenerator.circle(radius=Porosity_Radius, center=Porosity_Center,
                               grid=g)
# create a material id field, with material 1 in the circle (porosity)
# and mat Id 1 in the material
matId = np.int32(circle) + 1

#%% Build Amitex Geoemtry object

geom = AGeom(matId=matId)

#%% Compute internal boundary of circle
# create a layer with zone Id = 2 at the boundary of the porosity
# for instance to a prescribed behavior of the material there
UCT.add_boundary_layer_to_mat(Ageom=geom, matId=1, thickness=2)

#%% Show unit cell

plt.figure()
plt.imshow(geom.matId)

plt.figure()
plt.imshow(geom.zoneId)

#%% Write output files

VtkDir.mkdir(exist_ok=True)
geom.set_geometry_filename(filename=File_basename)
geom.write_files()