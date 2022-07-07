# state file generated using paraview version 4.4.0

# ----------------------------------------------------------------
# setup views used in the visualization
# ----------------------------------------------------------------

#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# Create a new 'Render View'
renderView1 = renderView1 = GetActiveViewOrCreate('RenderView') #CreateView('RenderView')
renderView1.ViewSize = [995, 766]
renderView1.AxesGrid = 'GridAxes3DActor'
renderView1.CenterOfRotation = [0.4999999888241291, 0.4999999888241291, 0.4999999888241291]
renderView1.StereoType = 0
renderView1.CameraPosition = [1.259728429773729, 1.1412747716485792, 2.9853805322550517]
renderView1.CameraFocalPoint = [0.4999999888241291, 0.4999999888241291, 0.4999999888241291]
renderView1.CameraViewUp = [-0.028710303287049904, 0.9699769817637622, -0.24149611453112008]
renderView1.CameraParallelScale = 0.6928203010894178
renderView1.Background = [0.32, 0.34, 0.43]

# ----------------------------------------------------------------
# setup the data processing pipelines
# ----------------------------------------------------------------

# create a new 'XML Rectilinear Grid Reader'
sphere_in_cube_RectilinearGridvtr = XMLRectilinearGridReader(FileName=['/tmp/sphere_in_cube_RectilinearGrid.vtr'])
sphere_in_cube_RectilinearGridvtr.PointArrayStatus = ['indice_including_sphere']

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

# show data from sphere_in_cube_RectilinearGridvtr
sphere_in_cube_RectilinearGridvtrDisplay = Show(sphere_in_cube_RectilinearGridvtr, renderView1)
# trace defaults for the display properties.
sphere_in_cube_RectilinearGridvtrDisplay.Representation = 'Surface With Edges'
sphere_in_cube_RectilinearGridvtrDisplay.ColorArrayName = ['POINTS', 'indice_including_sphere']
sphere_in_cube_RectilinearGridvtrDisplay.LookupTable = indiceincludingsphereLUT
sphere_in_cube_RectilinearGridvtrDisplay.Opacity = 0.54

# show color legend
sphere_in_cube_RectilinearGridvtrDisplay.SetScalarBarVisibility(renderView1, True)

# setup the color legend parameters for each legend in this view

# get color legend/bar for indiceincludingsphereLUT in view renderView1
indiceincludingsphereLUTColorBar = GetScalarBar(indiceincludingsphereLUT, renderView1)
indiceincludingsphereLUTColorBar.Title = 'indice_including_sphere'
indiceincludingsphereLUTColorBar.ComponentTitle = ''