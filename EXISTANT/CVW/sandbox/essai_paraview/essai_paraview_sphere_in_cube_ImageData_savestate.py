# state file generated using paraview version 4.4.0

# ----------------------------------------------------------------
# setup views used in the visualization
# ----------------------------------------------------------------

#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# Create a new 'Render View'
renderView1 = GetActiveViewOrCreate('RenderView') # CreateView('RenderView')
renderView1.ViewSize = [995, 766]
renderView1.AxesGrid = 'GridAxes3DActor'
renderView1.CenterOfRotation = [0.4000000059604645, 0.4000000059604645, 0.4000000059604645]
renderView1.StereoType = 0
renderView1.CameraPosition = [1.0760914238658223, 1.2094500397360668, 2.8603310409102964]
renderView1.CameraFocalPoint = [0.4000000059604645, 0.4000000059604645, 0.4000000059604645]
renderView1.CameraViewUp = [-0.029525889169948513, 0.9518771169108129, -0.30505438231614135]
renderView1.CameraParallelScale = 0.6928203333513783
renderView1.Background = [0.32, 0.34, 0.43]

# ----------------------------------------------------------------
# setup the data processing pipelines
# ----------------------------------------------------------------

# create a new 'XML Image Data Reader'
sphere_in_cube_ImageDatavtr = XMLImageDataReader(FileName=['/tmp/sphere_in_cube_ImageData.vtr'])
sphere_in_cube_ImageDatavtr.PointArrayStatus = ['indice_including_sphere']

# ----------------------------------------------------------------
# setup color maps and opacity mapes used in the visualization
# note: the Get..() functions create a new object, if needed
# ----------------------------------------------------------------

# get color transfer function/color map for 'indiceincludingsphere'
indiceincludingsphereLUT = GetColorTransferFunction('indiceincludingsphere')
indiceincludingsphereLUT.ScalarRangeInitialized = 1.0

# get opacity transfer function/opacity map for 'indiceincludingsphere'
indiceincludingspherePWF = GetOpacityTransferFunction('indiceincludingsphere')
indiceincludingspherePWF.ScalarRangeInitialized = 1

# ----------------------------------------------------------------
# setup the visualization in view 'renderView1'
# ----------------------------------------------------------------

# show data from sphere_in_cube_ImageDatavtr
sphere_in_cube_ImageDatavtrDisplay = Show(sphere_in_cube_ImageDatavtr, renderView1)
# trace defaults for the display properties.
sphere_in_cube_ImageDatavtrDisplay.Representation = 'Surface With Edges'
sphere_in_cube_ImageDatavtrDisplay.ColorArrayName = ['POINTS', 'indice_including_sphere']
sphere_in_cube_ImageDatavtrDisplay.LookupTable = indiceincludingsphereLUT
sphere_in_cube_ImageDatavtrDisplay.Opacity = 0.64
sphere_in_cube_ImageDatavtrDisplay.ScalarOpacityUnitDistance = 0.34641016151377557
sphere_in_cube_ImageDatavtrDisplay.Slice = 2

# show color legend
sphere_in_cube_ImageDatavtrDisplay.SetScalarBarVisibility(renderView1, True)

# setup the color legend parameters for each legend in this view

# get color legend/bar for indiceincludingsphereLUT in view renderView1
indiceincludingsphereLUTColorBar = GetScalarBar(indiceincludingsphereLUT, renderView1)
indiceincludingsphereLUTColorBar.Title = 'indice_including_sphere'
indiceincludingsphereLUTColorBar.ComponentTitle = ''