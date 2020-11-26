#!/usr/bin/env python
#-*- coding:utf-8 -*-

#  Copyright (C) 2010-2018  CEA/DEN
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

'''
Example of launch test
LaunchP4A --cmd test --pattern="test_4??_*py"

See https://deusyss.developpez.com/tutoriels/Python/hdf5/

Mode d'acces
  r  Lecture seule, le fichier doit exister
  r+ Lecture et écriture, le fichier doit exister
  w  Crée le fichier, ecrase si le fichier existe
  x  Crée le fichier, echoue si le fichier existe
  a  Lecture et ecriture, cree le fichier s'il n'existe pas
'''

import os
import sys
import platform
import unittest
import shutil

import h5py
import numpy as np
import py4amitex.debugpy.debug as DBG # Easy print stderr (for DEBUG only)

# import test.initializeTest # set PATH etc for test

_TMPDIR = os.path.realpath(os.path.expandvars("/tmp/${USER}/hdf5py"))
# print("ls -alt " + _TMPDIR)

verbose = False
verbosed = True # verbose unconditionnaly

class TestCase(unittest.TestCase):
  """Test the h5py"""
  
  def test_000(self):
    # one shot setUp() for this TestCase
    # DBG.push_debug(True)
    # SAT.setNotLocale() # test english

    # remove previous tests
    shutil.rmtree(_TMPDIR, ignore_errors=True)
    # create user test temporary directory
    os.makedirs(_TMPDIR, exist_ok=True)
    # print("remove done")
    return
    
  def test_005(self):
    nameFile = _TMPDIR + '/test_1.hdf5'
    # print("file " + nameFile)
    self.assertFalse(os.path.isfile(nameFile))
    f = h5py.File(nameFile, 'a')
    gr_1 = f.create_group('groupe_1')
    sgr_1 = f.create_group('/groupe_1/sous_groupe_1')
    f.close()
    self.assertTrue(os.path.isfile(nameFile))

  def test_010(self):
    nameFile = _TMPDIR + '/test_1.hdf5'
    f = h5py.File(nameFile, 'r')
    for element in f['/groupe_1'].items():
      self.assertEqual(str(element[0]), 'sous_groupe_1')
      self.assertEqual(str(element[1]), '<HDF5 group "/groupe_1/sous_groupe_1" (0 members)>')
      self.assertEqual(str(element[1].name), '/groupe_1/sous_groupe_1')
    f.close()
    if verbose: os.system("vitables "+nameFile)

  def test_015(self):
    nameFile = _TMPDIR + '/test_1.hdf5'
    f = h5py.File(nameFile, 'w')
    f.create_group('/groupe_1/sous_groupe_11')
    f.create_group('/groupe_2/sous_groupe_21')
    f.create_group('/groupe_2/sous_groupe_22')
    f.close()

    if verbose: os.system("vitables "+nameFile)

    f = h5py.File(nameFile, 'r')
    self.assertEqual(len(f), 2)
    i = 0
    # items() returns (name, object)
    for ng, g in f.items():
      i += 1
      if verbose: print('\ngroupe %s' % ng)
      self.assertEqual(ng, "groupe_%i" % i)
      j = 0
      for nsg, sg in g.items(): # or in f[ng].items():
        j += 1
        if verbose: print('  sous groupe full_name %s name %s' % (sg.name, nsg))
        self.assertEqual(sg.name, "/groupe_%i/sous_groupe_%i%i" % (i, i, j))
    f.close()
    if verbose: os.system("vitables "+nameFile)

  def test_030(self):
    # dataset as numpy matrice
    nameFile = _TMPDIR + '/test_1.hdf5'
    f = h5py.File(nameFile, 'w')
    group = f.create_group('/groupe_1/sous_groupe_11')
    aMat = np.ones((6, 4))
    aMat[0, :] = 2  # 2 first line
    aDataSet = group.create_dataset(name='demo_dataset', data=aMat, dtype="i8")
    if verbose: print("\naDataset\n%s" % aDataSet[:,:])
    f.flush()
    aDataSet[1, :] = 4 * aDataSet[0, :]  # 8 second line
    f.close()

    if verbose: os.system("vitables "+nameFile)

    f = h5py.File(nameFile, 'r')
    self.assertEqual(len(f), 1)
    group = f['/groupe_1/sous_groupe_11']
    newDataSet = group['demo_dataset']
    if verbose: print("\nnewDataset\n%s" % newDataSet[:,:])
    self.assertEqual(newDataSet[0,0], 2)
    self.assertEqual(newDataSet[1,0], 8)
    f.close()

    f = h5py.File(nameFile, 'a')
    group = f['/groupe_1/sous_groupe_11']
    newDataSet = group['demo_dataset']
    self.assertEqual(newDataSet[0,0], 2)
    self.assertEqual(newDataSet[1,0], 8)
    newDataSet[1, :] = 3 * newDataSet[1, :]  # 24 second line
    self.assertEqual(newDataSet[0,0], 2)
    self.assertEqual(newDataSet[1,0], 24)
    if verbose: print("\nnewDataset\n%s" % newDataSet[:,:])
    f.close() # 24 saved

    f = h5py.File(nameFile, 'r')  # read only
    group = f['/groupe_1/sous_groupe_11']
    newDataSet = group['demo_dataset']
    self.assertEqual(newDataSet[0,0], 2)
    self.assertEqual(newDataSet[1,0], 24) # 24 was saved
    with self.assertRaises(Exception):  # read only
      newDataSet[1, :] = 2 * newDataSet[1, :]  # read only no 48 second line
    self.assertEqual(newDataSet[1,0], 24)
    f.close()

  def test_035(self):
    nameFile = _TMPDIR + '/test_1.hdf5'
    f = h5py.File(nameFile, 'r')  # read only
    group = f['/groupe_1/sous_groupe_11']
    newArray = np.array(group['demo_dataset'])
    f.close()
    self.assertEqual(newArray[1,0], 24) # 24 was saved
    newArray[1, :] = 2 * newArray[1, :] # 48
    self.assertEqual(newArray[1,0], 48) # 48 was saved
    if verbose: print("\nnewArray\n%s" % newArray[:,:])

  def test_900(self):
    # big array traitement massif
    # with avoid f.close() safer if raise Exception
    nameFile = _TMPDIR + '/test_1.hdf5'
    nb = 1000
    aArray = np.ones((nb, nb)) # nb**2
    with h5py.File(nameFile, 'w') as f:
      aDataSet = f.create_dataset("big_array", data=aArray)

    # with-close done
    self.assertEqual('<Closed HDF5 dataset>', str(aDataSet))

    with h5py.File(nameFile, 'a') as f:
      g = f['/']
      newDataSet = g['big_array']
      if verbose: print(newDataSet.shape)
      self.assertEqual(newDataSet.shape, (nb, nb))
      newDataSet[:, 2] = 9 * newDataSet[:, 3]
      self.assertEqual(newDataSet[0,2], 9)

    with h5py.File(nameFile, 'r') as f:
      f = h5py.File(nameFile, 'r') # read
      g = f['/']
      newDataSet = g['big_array']
      self.assertEqual(newDataSet[0,2], 9)



  def test_999(self):
    # one shot tearDown() for this TestCase
    # SAT.setLocale() # end test english
    # DBG.pop_debug()

    # remove user test temporary directory
    # shutil.rmtree(_TMPDIR, ignore_errors=True)
    return
    
if __name__ == '__main__':
  verbose = False
  unittest.main(exit=False)
  pass

