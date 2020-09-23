#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
usage in paraview version 4.4.0
pvpython essai_paraview_cubeandsphere.py
paraview --help
paraview --script=essai_paraview_cubeandsphere.py

https://www.paraview.org/Wiki/ParaView/PythonRecipes
"""

from paraview import simple as PV
import time

def example_1():
  renderView1 = PV.GetActiveViewOrCreate('RenderView')

  box1 = PV.Box(XLength = 1.0, YLength = 1.0, ZLength = 1.0, Center = [0.5, 0.5, 0.5])
  sphere1 = PV.Sphere(ThetaResolution = 12, PhiResolution = 12, Center = [0., 0., 0.], Radius = 0.2)

  PV.Show(sphere1, renderView1)
  display1 = PV.Show(box1, renderView1)

  display1.SetRepresentationType('Surface With Edges')
  display1.DiffuseColor = [0.3, 0.0, 1.0]
  display1.Opacity = 0.1

  renderView1.ResetCamera()
  renderView1.CameraPosition = [-.5, 1.3, 5.0]
  # renderView1.CameraParallelScale = 1.0
  # renderView1.ResetCamera()
  PV.Render()

  time.sleep(3) # if pvpython wait a time

  servermanager = PV.servermanager
  # Create an animation scene
  scene = servermanager.animation.AnimationScene()
  # Add one view
  scene.ViewModules = [PV.GetActiveView()]

  # Create a cue to animate the StartTheta property
  cue = servermanager.animation.KeyFrameAnimationCue()
  cue.AnimatedProxy = PV.GetActiveSource()
  cue.AnimatedPropertyName = "StartTheta"
  # Add it to the scene's cues
  scene.Cues = [cue]

  # Create 2 keyframes for the StartTheta track
  keyf0 = servermanager.animation.CompositeKeyFrame()
  keyf0.Interpolation = 'Ramp'
  # At time = 0, value = 0
  keyf0.KeyTime = 0
  keyf0.KeyValues= [0]

  keyf1 = servermanager.animation.CompositeKeyFrame()
  # At time = 1.0, value = 200
  keyf1.KeyTime = 1.0
  keyf1.KeyValues= [180]

  # Add keyframes.
  cue.KeyFrames = [keyf0, keyf1]

  scene.PlayMode = 'Real Time'
  scene.Duration = 10
  scene.Play()

example_1()
