#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
py4amitex command launch sphere_in_cube.
create vtk voxel file with a sphere in a cube
cube is (0,0,0) to (sizecube, sizecube, sizecube)
sphere is centered at positionsphere
radius of sphere is radiussphere
cmd_sphere_in_cube.py is as simple pattern for future new commands
developer tips:
- copy file cmd_phere_in_cube.py as cmd_something.py in amitexpy folder
- modify cmd_something.py
- courage

example of use
LaunchP4A -c sphere_in_cube

n.b. utiliser vtk.vtkRenderer semble *au depart* plus simple que paraview, c'est un piege ou ne pas tomber.
"""

import os
import sys
import pprint as PP
import numpy as np


import py4amitex
import py4amitex.debugpy.debug as DBG  # Easy print stderr (for DEBUG only)
import py4amitex.loggerpy.loggingSimple as LOG
from py4amitex.returncodepy.returnCode import ReturnCode
from py4amitex.amitexpy.parserP4A import getNameCmdFromFileName
import py4amitex.amitexpy.DataP4A as DP4A

logger = LOG.getDefaultLogger()

__cmdname__ = getNameCmdFromFileName(__file__) # 'xxx' <- '.../cmd_xxx.py'

try:
  import vtk # avoid direct raise error (if not present as big prerequisite)
except:
  logger.critical("<critical>import vtk impossible, fix it.")
  vtk = None

def add_cmd_parser(parser):
  """
  using --cmd xxx then no need subtil add_subparsers method usage
  this way raise conflicts with previously common arguments set
  theses raises are catched for critical message
  as argparse.ArgumentError: argument ... conflicting option
  """
  parser.description = "command line interface for py4amitex --cmd %s stuff" % __cmdname__
  parser.add_argument(
    '-s', '--sizecube',
    help='size of cube',
    type=parser.filter_float_positive,
    default="1",
    metavar='sizeCube'
  )
  parser.add_argument(
    '-n', '--nbvoxels',
    help='number of voxels, for one dimension (same for x, y, z as total nb*nb*nb)',
    type=parser.filter_int_positive,
    default="5",
    metavar='nbVoxel'
  )
  parser.add_argument(
    '-p', '--positionsphere',
    help='number of voxels, for one dimension (same for x, y, z as total nb*nb*nb)',
    type=parser.filter_float_3d,
    default="[.5,.5,.5]",
    metavar='centerSphere'
  )
  parser.add_argument(
    '-r', '--radiussphere',
    help='number of voxels, for one dimension (same for x, y, z as total nb*nb*nb)',
    type=parser.filter_float_positive,
    default=".5",
    metavar='radiusSphere'
  )
  return


def dist2(p1, p2):
  """
  returns distance**2, avoid one useless sqrt for test
  """
  res = [(p1[0]-p2[0])**2, (p1[1]-p2[1])**2, (p1[2]-p2[2])**2]
  return sum(res)

def is_in_sphere(sphere, position):
  """
  simple calcul of position [x, y, z] is in sphere
  returns 1 if in, 0 if not
  not vectorized, not optimized
  0 as external, 1 as in sphere
  """
  if dist2(sphere.center, position) < sphere.radius2:
    return 1
  else:
    return 0


"""
vtk examples:
https://vtk.org/doc/nightly/html/classvtkStructuredGrid.html#details
https://lorensen.github.io/VTKExamples/site/Python/StructuredGrid/SGrid
https://www.paraview.org/Wiki/VTK/Examples/Cxx/Visualization/StructuredDataTypes
"""



def compute(iP4A):
  """
  iP4A is instance of RunnerP4A
  which contains all caller data informations
  """

  # this command begin common stuff
  parser = iP4A.parser # shortcut
  args = iP4A.args # shortcut
  logger.info("<data>Begin %s.compute with args %s<reset>" % (__cmdname__, args))

  # this command more options
  add_cmd_parser(parser)
  logger.info("<header>%s<reset>\n%s" % (__doc__, parser.format_help()))
  options = parser.parse_args(args)
  logger.info("%s options:\n\n<data>%s<reset>\n" %
              (__cmdname__, parser.getNamespaceStr(options)))

  compute_vtkImageData(options)
  compute_RectilinearGrid(options)

  logger.info("<data>End %s.compute" % __cmdname__)
  return ReturnCode("OK", "End %s.compute" % __cmdname__)


def compute_vtkImageData(options):
  """
  iP4A is instance of RunnerP4A
  which contains all caller data informations
  option choice vtkImageData
  """

  # this command real stuff

  ############################################
  # use of python numpy API for arrays stuff
  ############################################

  nbv = options.nbvoxels
  siz = options.sizecube
  dx = siz/nbv
  dxs2 = dx/2.
  sphere = DP4A.DataP4A()  # use dataP4A as easy usage instance of one sphere
  sphere.radius = options.radiussphere
  # leads to tangent only one vovel (if odd nv voxels) as simple test
  # sphere.radius = options.radiussphere -dx/(2.+0.0001)
  sphere.radius2 = sphere.radius**2 # avoid useless **2 **.5 in many voxel iterations
  sphere.center = options.positionsphere

  dims = [nbv]*3
  voxels = np.ones(shape=dims, dtype=np.int8)  # np.int8 could be enough
  x = np.zeros(shape=[nbv], dtype=np.float64)
  dx = siz/nbv
  dxs2 = dx/2.
  for i in range(nbv):
    x[i] = dx*i + dxs2
  DBG.write("x" , x, True)

  # not vectorized, not optimized, but in numpy array
  for i, xi in enumerate(x):
    for j, yi in enumerate(x):
      for k, zi in enumerate(x):
        # 0 as external, 1 as in (the only one) sphere
        voxels[i, j, k] = is_in_sphere(sphere, [xi, yi, zi])
        pass

  # DBG.write("dir(vtk)" , [i for i in dir(vtk) if "mage" in i and "Writer" in i], True)
  DBG.write("voxels" , voxels, True)

  ############################################
  # use of vtk API for vtk stuff
  ############################################

  # use vtkImageData, it is my choice.
  # could be other vtkRectilinearGrid.
  data = vtk.vtkImageData()
  dimx, dimy, dimz = dims
  ntot = dimx * dimy * dimz
  data.SetDimensions(dimx, dimy, dimz)
  data.SetSpacing(dx, dx, dx)
  data.SetOrigin(0.0, 0.0, 0.0)

  # as field indice_including_sphere, future index of including sphere(s)
  vectors = vtk.vtkIntArray()
  vectors.SetName("indice_including_sphere")
  vectors.SetNumberOfComponents(1)
  vectors.SetNumberOfValues(ntot) # not mandatory as InsertNextValue
  data.GetPointData().AddArray(vectors)

  ii = 0
  for i, xi in enumerate(x):
    for j, yi in enumerate(x):
      for k, zi in enumerate(x):
        # vectors.InsertNextValue(voxels[i, j, k])
        vectors.SetValue(ii, voxels[i, j, k])
        ii += 1

  # try xml
  writer = vtk.vtkXMLImageDataWriter()
  # DBG.write("dir(writer)", dir(writer), True)
  writer.SetDataModeToAscii()  # more readable for eyes but larger files
  # writer.SetByteOrderToBigEndian()
  filevtk = "/tmp/%s_ImageData.vtr" % __cmdname__
  writer.SetFileName(filevtk)
  writer.SetInputData(data)
  logger.info("create file xml %s as type %s" % (filevtk, type(data)))
  writer.Write()

  if False: # seems vtkImageDataWriter do not exist.
    # try legacy
    writer = vtk.vtkImageDataWriter()
    # DBG.write("dir(writer)", dir(writer), True)
    filevtk = "/tmp/%s_ImageData.vtk" % __cmdname__
    writer.SetFileName(filevtk)
    writer.SetInputData(data)
    logger.info("create file legacy %s as type %s" % (filevtk, type(data)))
    writer.Write()

  logger.info("""one visualisation could be:
'paraview --script=./py4amitex/sandbox/essai_paraview/essai_paraview_sphere_in_cube_ImageData_savestate.py'""")
  # n.b. utiliser vtk.vtkRenderer semble au depart plus simple que paraview, c'est un piege ou ne pas tomber.
  return




def compute_RectilinearGrid(options):
  """
  iP4A is instance of RunnerP4A
  which contains all caller data informations
  option choice vtkRectilinearGrid
  """

  # this command real stuff

  ############################################
  # use of python numpy API for arrays stuff
  ############################################

  nbv = options.nbvoxels
  siz = options.sizecube
  dx = siz/nbv
  dxs2 = dx/2.
  sphere = DP4A.DataP4A()  # use dataP4A as easy usage instance of one sphere
  sphere.radius = options.radiussphere
  # leads to tangent only one vovel (if odd nv voxels) as simple test
  # sphere.radius = options.radiussphere -dx/(2.+0.0001)
  sphere.radius2 = sphere.radius**2 # avoid useless **2 **.5 in many voxel iterations
  sphere.center = options.positionsphere

  dims = [nbv]*3
  voxels = np.ones(shape=dims, dtype=np.int8)  # np.int8 could be enough
  x = np.zeros(shape=[nbv], dtype=np.float64)
  dx = siz/nbv
  dxs2 = dx/2.
  for i in range(nbv):
    x[i] = dx*i + dxs2
  DBG.write("x" , x, True)

  # not vectorized, not optimized, but in numpy array
  for i, xi in enumerate(x):
    for j, yi in enumerate(x):
      for k, zi in enumerate(x):
        # 0 as external, 1 as in (the only one) sphere
        voxels[i, j, k] = is_in_sphere(sphere, [xi, yi, zi])
        pass

  DBG.write("voxels" , voxels, True)

  ############################################
  # use of vtk API for vtk stuff
  ############################################

  # https://vtk.org/Wiki/VTK/Writing_VTK_files_using_python
  # https://www.paraview.org/pipermail/paraview/2013-April/027979.html
  # https://www.paraview.org/Wiki/VTK/Examples/Python/RectilinearGrid/vtkRectilinearGrid

  # could be vtkStructuredPoints, but only for cubic voxels
  # vtkRectilinearGrid is more complex, here for example...
  data = vtk.vtkRectilinearGrid()
  dimx, dimy, dimz = dims
  ntot = dimx * dimy * dimz
  data.SetExtent(0, dimx-1, 0, dimy-1, 0, dimz-1)
  xCoords = vtk.vtkDoubleArray()
  xCoords.SetNumberOfComponents(1)
  for i, xi in enumerate(x):
    xCoords.InsertNextValue(xi)

  # but obviously regular 3d grid
  data.SetXCoordinates(xCoords)
  data.SetYCoordinates(xCoords)
  data.SetZCoordinates(xCoords)

  # as field indice_including_sphere, future index of including sphere(s)
  vectors = vtk.vtkIntArray()
  vectors.SetName("indice_including_sphere")
  vectors.SetNumberOfComponents(1)
  vectors.SetNumberOfValues(ntot) # not mandatory as InsertNextValue
  data.GetPointData().AddArray(vectors)

  ii = 0
  for i, xi in enumerate(x):
    for j, yi in enumerate(x):
      for k, zi in enumerate(x):
        # vectors.InsertNextValue(voxels[i, j, k])
        vectors.SetValue(ii, voxels[i, j, k])
        ii += 1

  # try xml
  writer = vtk.vtkXMLRectilinearGridWriter()
  # DBG.write("dir(writer)", dir(writer), True)
  writer.SetDataModeToAscii()  # more readable for eyes but larger files
  writer.SetByteOrderToBigEndian()
  filevtk = "/tmp/%s_RectilinearGrid.vtr" % __cmdname__
  writer.SetFileName(filevtk)
  writer.SetInputData(data)
  logger.info("create file xml %s as type %s" % (filevtk, type(data)))
  writer.Write()

  # try legacy
  writer = vtk.vtkRectilinearGridWriter()
  # DBG.write("dir(writer)", dir(writer), True)
  filevtk = "/tmp/%s_RectilinearGrid.vtk" % __cmdname__
  writer.SetFileName(filevtk)
  writer.SetInputData(data)
  logger.info("create file legacy %s as type %s" % (filevtk, type(data)))
  writer.Write()

  logger.info("""one visualisation could be:
'paraview --script=./py4amitex/sandbox/essai_paraview/essai_paraview_sphere_in_cube_RectilinearGrid_savestate.py'""")
  # n.b. utiliser vtk.vtkRenderer semble au depart plus simple que paraview, c'est un piege ou ne pas tomber.
  return

