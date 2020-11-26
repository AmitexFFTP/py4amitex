#!/usr/bin/env python
# -*- coding:utf-8 -*-

#  Copyright (C) 2010-2020  CEA/DEN
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

# https://www.hdfgroup.org/2018/06/hdf5-or-how-i-learned-to-love-data-compression-and-partial-i-o/

'''
Example of launch test
LaunchP4A --cmd test --pattern="test_35?_*py"

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
import pandas as pd

import py4amitex.amitexpy.DataP4A as DP4A
import py4amitex.debugpy.debug as DBG  # Easy print stderr (for DEBUG only)

import py4amitex.loggerpy.loggingSimple as LOG

logger = LOG.getDefaultLogger()

# import test.initializeTest # set PATH etc for test

_TMPDIR = os.path.realpath(os.path.expandvars("/tmp/${USER}/hdf5py"))
# print("ls -alt " + _TMPDIR)

verbose = False
verbosed = True  # verbose unconditionnaly

dataJson_1 = r'''
{
  "aTrue": true,
  "aFalse": false,
  "pi": 3.141592653589793,
  "nb_heroes": 2,
  "heroes": [
    {
      "name": "Tintin",
      "job": "reporter"
    },
    {
      "name": "Haddock",
      "job": "capitaine"
    }
  ]
}
'''

class TestCase(unittest.TestCase):
  """Test the h5py plus DataP4A"""

  def create_data(self, pddf=False):
    a = DP4A.DataP4A()
    a.loadStrJson(dataJson_1, verbosed)

    # a.my_array
    a.my_array = np.ones((6, 4))
    a.my_array._value[:,0] = 6.4

    a.deeper = DP4A.DataP4A()

    # a.deeper.my_other_array_1
    a.deeper.my_other_array_1 = np.ones((7, 5))
    a.deeper.my_other_array_1._value[:,0] = 7.5

    # a.deeper.my_other_array_2
    a.deeper.my_other_array_2 = np.ones((8, 6))
    a.deeper.my_other_array_2._value[:,0] = 8.6

    if pddf == True:
      a.deeper.my_pandas_dataframe = \
        pd.DataFrame({'AA': [11, 22, 33], 'BB': [44, 55., 66]})

    return a


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

  def test_100(self):
    # https://docs.h5py.org/en/2.10.0/strings.html
    aMat1 = np.ones((6, 4))
    aMat2 = np.ones((3, 2))

    nameFile = _TMPDIR + '/test_1.hdf5'
    f = h5py.File(nameFile, 'w')
    gdat = f.create_group('/data')
    # gdat.attrs["dataJson"] = np.string_(dataJson_1)
    gdat.create_dataset(name="dataJson", data=np.string_(dataJson_1))
    # gdat.create_dataset("dataJson", data=dataJson_1)
    # gdat.attrs.create("dataJson", data=dataJson_1)

    garr = f.create_group('/arrays')
    garr.create_dataset(name='mat_1', data=aMat1, dtype="i8")
    garr.create_dataset(name='mat_2', data=aMat2, dtype="f8")

    f.close()
    if verbose: os.system("vitables " + nameFile)

    # or... tricky... better rewrite vitables.start def gui wit arguments...
    # from vitables.start import gui
    # sys.argv = ['/volatile/common/miniconda3/envs/py3/bin/vitables', '-m', 'a', '/tmp/christian/hdf5py/test_1.hdf5']
    # gui()

  def test_110(self):
    # pandas dataframe in HDF5 with pd.HDFStore
    # https://pandas.pydata.org/pandas-docs/stable/user_guide/io.html#io-hdf5
    nameFile = _TMPDIR + '/test_pandas_dataframe.hdf5'
    store = pd.HDFStore(nameFile, "w")
    df = pd.DataFrame({'AA': [11, 22, 33], 'BB': [44, 55., 66]}) #, 'CC': [77, 88, 99]})
    if verbose: print("file %s df is:\n%s" % (nameFile, df))

    # bofbof df.to_hdf('data.h5', key='df', mode='w')

    store.put('df', df) # or store['df'] = df
    store.info()
    store.close()

    store2 = pd.HDFStore(nameFile, "r")
    df2 = store2.get('df')
    store2.close()
    if verbose: print("file %s df2 is:\n%s" % (nameFile, df2))

    self.assertEqual(str(df), str(df2))
    # creates df wit
    if verbose: os.system("vitables %s &" % nameFile)


  def test_200(self):
    # this is test model of dumpFileHdf5 method using dumpStrJsonHdf5
    # There are other solutions...
    nameFile = _TMPDIR + '/test_1.hdf5'

    a = self.create_data()
    DBG.write("test_200 a", a, verbose)

    with h5py.File(nameFile, 'w') as f:
      gdat = f.create_group('/data')
      aStr = a._dumpStrJsonHdf5()
      gdat.create_dataset(name="dataJson", data=np.string_(aStr))
      garr = f.create_group('arrays')
      for p, val in DP4A._HDF5_ARRAYS.items():
        # print('create %s' % p)
        garr.create_dataset(name=p, data=val)

    if verbose: os.system("vitables " + nameFile)

  def test_220(self):
    # this is test model of dumpFileHdf5 method using dumpStrJsonHdf5
    # There are other solutions...
    # compression nb = 10000
    # from 763M 11 nov.  11:52 /tmp/christian/hdf5py/test_1.hdf5
    # to   1,8M 11 nov.  11:53 /tmp/christian/hdf5py/test_1.hdf5
    nameFile = _TMPDIR + '/test_1.hdf5'

    a = DP4A.DataP4A()
    a.loadStrJson(dataJson_1, verbosed)
    nb = 100

    a.big_array = np.ones((nb, nb))
    a.big_array._value[:,0] = 7.

    # not so big log for array
    DBG.write("test_220 a", a.dumpStrJsonResume(), verbose)

    with h5py.File(nameFile, 'w') as f:
      gdat = f.create_group('/data')
      aStr = a._dumpStrJsonHdf5()
      gdat.create_dataset(name="dataJson", data=np.string_(aStr))
      garr = f.create_group('arrays')
      for p, val in DP4A._HDF5_ARRAYS.items():
        # print('create %s' % p)
        # with compression
        garr.create_dataset(name=p, data=val, compression='gzip', compression_opts=9)
        # without compression
        # garr.create_dataset(name=p, data=val)

    if verbose:
      os.system('ls -alth %s' % nameFile)
      os.system("vitables " + nameFile)

  def test_300(self):
    # this is test model of dumpFileHdf5 method using dumpStrJsonHdf5
    # There are other solutions...
    # verbose = True
    nameFile = _TMPDIR + '/test_1.hdf5'

    a = self.create_data()
    a.dumpFileHdf5(nameFile, display=False, compression=None)
    aStra = a.dumpStrJson()
    if verbose: logger.info("test_300 a\n%s" % aStra)
    if verbose: os.system("vitables %s &" % nameFile)

    b = DP4A.DataP4A()
    b.loadFileHdf5(nameFile, verbose=verbose)
    aStrb = b.dumpStrJson()
    if verbose: logger.info("test_300 a\n%s" % aStrb)

    self.assertEqual(aStra, aStrb)

    for p in "my_array deeper.my_other_array_1 deeper.my_other_array_2".split():
      aa = a._getPyPath(p)._value
      bb = b._getPyPath(p)._value
      if verbose:
        print('\naa %s%s\n' % (p, aa.shape), aa)
        print('\nbb %s%s\n' % (p, bb.shape), bb)
      self.assertNotEqual(id(aa), id(bb))
      self.assertTrue(np.array_equal(aa, bb))

      aa[0, 0] = 123
      self.assertFalse(np.array_equal(aa, bb))

  def test_350(self):
    # this is test model of dumpFileHdf5 method using dumpStrJsonHdf5
    # There are other solutions...
    # verbose = True
    nameFile = _TMPDIR + '/test_1.hdf5'

    a = self.create_data()
    df = pd.DataFrame({'AA': [11, 22, 33], 'BB': [44, 55., 66]})
    a.deeper.my_pandas_dataframe = df
    a.deeper.another_list = []   # casted as DP4A.DataP4AList
    self.assertEqual(a.deeper.another_list.__class__, DP4A.DataP4AList)

    df2 = df.copy()
    df2["CC"] = 22  # append column
    a.deeper.another_list.append(df2)
    df2 = df.copy()
    df2["CC"] = 33  # append column
    a.deeper.another_list.append(df2)

    a.dumpFileHdf5(nameFile, display=False, compression=None)

    aStra = a.dumpStrJson()
    if verbose:
      logger.info("test_350 a.dumpStrJson()\n%s" % a.dumpStrJson())
      logger.info("test_350 a.dumpStrJsonResume()\n%s" % a.dumpStrJsonResume())
    if verbose: os.system("vitables %s &" % nameFile)


    b = DP4A.DataP4A()
    b.loadFileHdf5(nameFile, verbose=verbose)
    aStrb = b.dumpStrJson()
    if verbose:
      DBG.write("test_300 a", aStra, True)
      DBG.write("test_300 b", aStrb, True)

    self.assertEqual(aStra, aStrb)


    for p in "my_array deeper.my_other_array_1 deeper.my_other_array_2".split():
      aa = a._getPyPath(p)._value
      bb = b._getPyPath(p)._value
      if verbose:
        print('\nnumpy read aa %s%s\n' % (p, aa.shape), aa)
        print('\nnumpy read bb %s%s\n' % (p, bb.shape), bb)
      self.assertNotEqual(id(aa), id(bb))
      self.assertTrue(np.array_equal(aa, bb))

      aa[0, 0] = 123
      self.assertFalse(np.array_equal(aa, bb))

    for p in "deeper.my_pandas_dataframe".split():
      aa = a._getPyPath(p)._value
      bb = b._getPyPath(p)._value
      if verbose:
        print('\npandas read aa %s%s\n' % (p, aa.shape), aa)
        print('\npandas read bb %s%s\n' % (p, bb.shape), bb)
      self.assertNotEqual(id(aa), id(bb))
      self.assertEqual(str(aa), str(bb))




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

