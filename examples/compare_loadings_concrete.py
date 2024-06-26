#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Script to visualize the outputs of two validation tests of AMITEX,
that simulate the mechanical response of a concrete material unit cell:

    - the 'beton_fluage' test, where the unit cell is submitted to a constant
      tension stress along the X axis (creep test)

    - the 'beton_relax' test, where the unit cell is submitted to a constant
      imposed strain, with a compression along the X axis (relaxation test)

This script uses the Amitex output files processing tools implemented in py4amitex.

The following plots will be soon replaced by high level routines to :
    - plot a mean value over time
    - plot a mean value time serie in function of another mean value serie
    - plot a 22D slice of a specific field

@author: amarano
"""
from config import AMITEX_TEST_RES_DIR

import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

from py4amitex.output.amitexoutput import AmitexOutput as AO

# options for plot fonts
fax = {'family': 'serif', 'color':  'black', 'weight': 'normal',
        'size': 24}
# fticks = {'family': 'serif', 'color':  'black', 'weight': 'normal',
#         'size': 16}

#%% set test cases path

beton_fluage_files = ( Path(AMITEX_TEST_RES_DIR) / "beton_fluage_64"
                          / "beton_fluage_64")
beton_relax_files = ( Path(AMITEX_TEST_RES_DIR) / "beton_relax_65"
                          / "beton_relax_65")

#%% read outputs from beton fluage test case

print('Loading data from "beton_fluage" test case...')

Fluage_output = AO(output_basename=beton_fluage_files)
Fluage_output.load_mean_values()
Fluage_output.load_all_fields()

#%% read outputs from beton relax test case

print('Loading data from "beton_relax" test case...')

Relax_output = AO(output_basename=beton_relax_files)
Relax_output.load_mean_values()
Relax_output.load_all_fields()

#%% Get mean values from both cases

# get axial stress and axial strain along X for fluage case
# Fluage_stress = Fluage_output.get_mean_values('stress')[:,0]
Fluage_strain = Fluage_output.get_mean_values('strain')[:,0]*100
Fluage_time = Fluage_output.get_mean_values('time')[:]

# get axial stress and axial strain along X for relax case
Relax_stress = Relax_output.get_mean_values('stress')[:,0]
# Relax_strain = Relax_output.get_mean_values('strain')[:,0]*100
Relax_time = Relax_output.get_mean_values('time')[:]

#%% Plot creep strain over time

# create image
im1 = plt.figure(1, (8,5), 200)
# prepare axes titles
plt.xlabel('time (s)', fontweight='bold', fontdict=fax)
plt.ylabel(r'$\varepsilon_{xx}$ (%)', fontweight='bold', fontdict=fax)
# plot both curves
plt.plot(Fluage_time, Fluage_strain, label='Fluage', linewidth=2)
plt.title('Evolution of axial strain during creep test')

#%% Plot relaxation stress over time

# create image
im2 = plt.figure(2, (8,5), 200)
# prepare axes titles
plt.xlabel('time (s)', fontweight='bold', fontdict=fax)
plt.ylabel(r'$\sigma_{xx}$ (Pa)', fontweight='bold', fontdict=fax)
# plot both curves^
plt.plot(Relax_time, Relax_stress, label='Fluage', linewidth=2)
plt.title('Evolution of axial stress during relaxation test')


#%% Get axial strain field at end of creep test and show it

# get central (x,y) slice of strain field
Fluage_strain_field = Fluage_output.get_strain_field(component='xx',
                                                     increment=32)[:,:,32]
# create image
plt.figure(3, (8,5), 200)
im3 = plt.imshow(Fluage_strain_field*100)
plt.colorbar(im3, format="%1.1f", label=r'$\varepsilon_{xx}$ (%)')
plt.title('Axial strain field at end of creep test')

#%% Get axial stress field at end of creep test and show it

# get central (x,y) slice of strain field
Fluage_stress_field = Fluage_output.get_stress_field(component='xx',
                                                     increment=32)[:,:,32]
# create image
plt.figure(4, (8,5), 200)
im4 = plt.imshow(Fluage_stress_field*1e-6)
plt.colorbar(im4, format="%8.2f", label=r'$\sigma_{xx}$ (MPa)')
plt.title('Axial stress field at end of creep test')

#%% Get axial stress field at end of relaxation test and show it


# get central (x,y) slice of strain field
Relax_stress_field = Relax_output.get_stress_field(component='xx',
                                                    increment=32)[:,:,32]
# create image
plt.figure(5, (8,5), 200)
im5 = plt.imshow(Relax_stress_field*1e-6)
plt.colorbar(im5, format="%8.2f", label=r'$\sigma_{xx}$ (MPa)')
plt.title('Axial stress field at end of relaxation test')

