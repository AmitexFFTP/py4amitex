#!/usr/bin/env python
# -*- coding:utf-8 -*-


import os
import sys
import unittest
import pprint as PP
import math
import numpy as np

import py4amitex.debugpy.debug as DBG
import py4amitex.amitexpy.DataP4A as DP4A

verbose = False # False as production, set True if debug unittest
verbosed = True # use True if debug unittest

# dict contains list for test
_dataPy_1 = '''
# you have to KISS, it is DATA in python dict syntax, not program
import math

heroes = [
  {"name": "Tintin", "job": "reporter"},
  {"name": "Haddock", "job": "capitaine"},
]

DATA_IN = {
    "pi": math.pi,
    "nothing": None,
    "nb_heroes": len(heroes),
    "heroes": heroes,
}
'''

# dict contains dict for test
_dataPy_2 = '''
heroes = {
  "Tintin": {"job": "reporter"},
  "Haddock": {"job": "capitaine"},
}

DATA_IN = {
    "nb_heroes": len(heroes),
    "heroes": heroes,
}
'''

_dataPy_3 = '''
DATA_IN = {
    "v1": {"v1_1": 1.1, "v1_2": 1.2},
    "v2": {"v2_1": 2.1, "v2_2": 2.2},
    "v3": {"v3_1": {"v3_1_1": 3.11, "v3_1_2": 3.12, "v3_1_3": 3.13}},
    "vx": [
      {"vx_1": {"vx_1_1": 0.11, "vx_1_2": 0.12, "vx_1_3": 0.13}},
      {"vx_2": {"vx_2_1": 0.21, "vx_2_2": 0.22, "vx_2_3": 0.23}},
    ]
}
'''


# dict contains list for test
# new lines in python string for json syntax, user HAVE TO USE  --> r''' <---
_dataJson_1 = r'''
{
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


_keys_dataPy_1 = "pi nb_heroes heroes".split()



class TestCase(unittest.TestCase):
  """Test the DataP4A.py, recursive implementation"""

  def test_000(self):
    # one shot setUp() for this TestCase
    # DBG.write("assert unittest", [a for a in dir(self) if "assert" in a], verbosed)
    pass

  def test_005(self):
    value = True
    self.assertTrue(bool == value.__class__)
    value = 1
    self.assertTrue(int == value.__class__)
    value = 1.
    self.assertTrue(float == value.__class__)
    value = 'x'
    self.assertTrue(str == value.__class__)
    value = None
    self.assertTrue(None.__class__ == value.__class__)

  def test_010(self):
    DBG.write("test_010", "", verbose)
    aStr = """
DATA_IN = {
  "pi": 3.141592653589793,
  "heroes": [
    {
      "name": Oops, #### error to test raises
      "job": "reporter"
    },
  ]
}
"""
    a = DP4A.DataP4A()
    # as a.loadStrPy(aStr, verbose)
    self.assertRaises(Exception, a.loadStrPy, aStr, verbose)

  def test_012(self):
    DBG.write("test_012", "", verbose)
    # missed ',' after Tintin
    aStr = """
{
  "pi": 3.141592653589793,
  "heroes": [
    {
      "name": "Tintin"
      "job": "reporter"
    }
  ]
}
"""
    a = DP4A.DataP4A()
    # as a.loadStrJson(aStr, verbose)
    self.assertRaises(Exception, a.loadStrJson, aStr, verbose)

  def test_015(self):
    DBG.write("test_015", "", verbose)
    a = DP4A.DataP4A()
    a.none = None
    a.true = True
    a.false = False
    a.int = 1
    a.float = 2.
    a.true = True
    a.false = False
    a.list = [1, 2]
    a.dict = {'v1': 11., 'v2': 22.}
    DBG.write("a", a, verbose)
    DBG.write("a.dumpStrJson()", a.dumpStrJson(), verbose)
    DBG.write("a.dumpStrPy()", a.dumpStrPy(), verbose)

  def test_017(self):
    # expected send warning message 'hope you know what you are doing'
    import py4amitex.loggerpy.loggingSimple as LOG
    logger = LOG.getDefaultLogger()
    # help(logger)
    oldlevel = logger.getEffectiveLevel()
    logger.setLevel("ERROR")
    a = DP4A.DataP4A()
    a.dict = {'v1': 11., 'v2': 22., 'np_array': np.array([10,11,12])}

    # expected send error message 'Unexpected leaf data type'
    logger.setLevel("CRITICAL")
    DBG.write("a", str(a), verbose)
    DBG.write("a.dumpStrJsonResume()", a.dumpStrJsonResume(), verbose)
    # DBG.write("a.dumpStrJson()", a.dumpStrJson(), verbose)
    # DBG.write("a.dumpStrPy()", a.dumpStrPy(), verbose)
    logger.setLevel(oldlevel)


  ### loadStrPy use python syntax for data, no limits!
  ### avoid if then else please: it is DATA, not program
  def test_022(self):
    DBG.write("test_022", "", verbose)
    DBG.write("_dataPy_1", _dataPy_1, verbose)
    a = DP4A.DataP4A()
    a.loadStrPy(_dataPy_1)
    aStr = PP.pformat(a)
    DBG.write("a", aStr, verbose)
    self.assertTrue("'nothing'" in aStr)
    self.assertTrue("'heroes': [" in aStr)
    self.assertTrue("'nb_heroes'" in aStr)
    self.assertTrue("'Tintin'" in aStr)
    self.assertTrue("'Haddock'" in aStr)

  ### dumpStrJson
  def test_024(self):
    DBG.write("test_024", "", verbose)
    a = DP4A.DataP4A()
    DBG.write("_dataPy_1", _dataPy_1, verbose)
    a.loadStrPy(_dataPy_1)
    aStr = a.dumpStrJson()
    DBG.write("a.dumpStrJson()", aStr, verbose)
    self.assertTrue('"pi": 3.14159' in aStr)
    self.assertTrue('"nothing": null' in aStr)
    self.assertTrue('"nb_heroes": 2,' in aStr)
    self.assertTrue('"heroes": [' in aStr)
    self.assertTrue('"name": "Tintin",' in aStr)
    self.assertTrue('"name": "Haddock",' in aStr)

  def test_026(self):
    DBG.write("test_026", "", verbose)
    a = DP4A.DataP4A()
    DBG.write("_dataPy_2", _dataPy_2, verbose)
    a.loadStrPy(_dataPy_2)
    aStr = a.dumpStrJson()
    DBG.write("a.__repr__()", a.__repr__(), verbose)
    DBG.write("a.dumpStrJson()", aStr, verbose)
    self.assertEqual('reporter', a.heroes.Tintin.job)


  ### loadStrJson
  def test_042(self):
    DBG.write("test_042", "", verbose)
    a = DP4A.DataP4A()
    DBG.write("_dataJson_1", _dataJson_1, verbose)
    a.loadStrJson(_dataJson_1)
    aStr = a.__repr__()
    DBG.write("a.__repr__()", aStr, verbose)
    aStr = a.dumpStrJson()
    DBG.write("a.dumpStrJson()", aStr, verbose)
    self.assertTrue('"pi": 3.14159' in aStr)
    self.assertTrue('"nb_heroes": 2,' in aStr)
    self.assertTrue('"heroes": [' in aStr)
    self.assertTrue('"name": "Tintin",' in aStr)
    self.assertTrue('"name": "Haddock",' in aStr)

  ### class.key coding way: a.heroes[0].name
  def test_222(self):
    DBG.write("test_222", "", verbose)
    a = DP4A.DataP4A()
    DBG.write("_dataPy_1", _dataPy_1, verbose)
    a.loadStrPy(_dataPy_1)
    DBG.write("a", a, verbose)
    self.assertEqual(math.pi, a.pi)
    self.assertEqual(2, a.nb_heroes)
    self.assertEqual("Tintin", a.heroes[0]["name"])
    self.assertEqual("Haddock", a.heroes[1]["name"])
    self.assertEqual("Tintin", a.heroes[0].name)
    self.assertEqual("Haddock", a.heroes[1].name)

  # test python path
  def test_510(self):
    DBG.write("test_510", "", verbose)
    a = DP4A.DataP4A()
    DBG.write("_dataPy_1", _dataPy_1, verbose)
    a.loadStrPy(_dataPy_1)
    DBG.write("a", a, verbose)
    self.assertEqual('root.heroes[0]', a.heroes[0].getPythonPath(root='root'))
    self.assertEqual('root.heroes[1]', a.heroes[1].getPythonPath(root='root'))

  def test_520(self):
    DBG.write("test_520", "", verbose)
    a = DP4A.DataP4A()
    DBG.write("_dataPy_3", _dataPy_3, verbose)
    a.loadStrPy(_dataPy_3)
    DBG.write("a", a, verbose)
    DBG.write("a.dumpStrJson()", a.dumpStrJson(), verbose)
    DBG.write("a PP.pformat", PP.pformat(a), verbose)
    self.assertEqual('root.v1', a.v1.getPythonPath(root='root'))
    self.assertEqual('root.v3.v3_1', a.v3.v3_1.getPythonPath(root='root'))
    self.assertEqual('root.vx[0].vx_1', a.vx[0].vx_1.getPythonPath(root='root'))
    self.assertEqual('root.vx[1].vx_2', a.vx[1].vx_2.getPythonPath(root='root'))

    self.assertEqual('root.vx[0].vx_1.vx_1_1', a.vx[0].vx_1.vx_1_1.getPythonPath(root='root'))
    self.assertEqual('root.vx[1].vx_2.vx_2_2', a.vx[1].vx_2.vx_2_2.getPythonPath(root='root'))

  # manipulate DataP4A in python script
  # what we could do... may be it is 'too much' for python newbies
  # may be only use loadStrPy or loadStrJson and validate
  def test_610(self):
    DBG.write("test_610", "", verbose)
    a = DP4A.DataP4A()
    a.loadStrPy(_dataPy_1)

    # create from scratch, not loadStrPy or loadStrJson
    b = DP4A.DataP4A()
    b["FirstItem"] = DP4A.DataP4A({"Firstvalue": 11})
    b["SecondItem"] = DP4A.DataP4A({"Secondvalue": 22})
    DBG.write("b", b, verbose)
    DBG.write("b.__repr__()", b.__repr__(), verbose)
    self.assertEqual(b, b.FirstItem._parent)
    self.assertEqual(b, b.SecondItem._parent)
    self.assertEqual('root.FirstItem', b.FirstItem.getPythonPath(root='root'))
    self.assertEqual('root.SecondItem', b.SecondItem.getPythonPath(root='root'))

    # create from scratch, not loadStrPy or loadStrJson (id as previous)
    b = DP4A.DataP4A()
    b.FirstItem = {"Firstvalue": 11}
    b.SecondItem = {"Secondvalue": 22}
    DBG.write("b", b, verbose)
    DBG.write("b.__repr__()", b.__repr__(), verbose)
    self.assertEqual(b, b.FirstItem._parent)
    self.assertEqual(b, b.SecondItem._parent)
    self.assertEqual('root.FirstItem', b.FirstItem.getPythonPath(root='root'))
    self.assertEqual('root.SecondItem', b.SecondItem.getPythonPath(root='root'))

    for i in range(0, 55*5, 55):
      b.FirstItem.Firstvalue = i
      b.SecondItem.Secondvalue = i*2
      DBG.write("b", b, verbose)
      aStr = b.dumpStrJson()
      DBG.write("b.dumpStrJson()", aStr, verbose)
      self.assertTrue("Firstvalue: %i" % i, aStr)
      self.assertTrue("Secondvalue: %i" % i*2, aStr)
      self.assertEqual(i, b.FirstItem.Firstvalue)
      self.assertEqual(i*2, b.SecondItem.Secondvalue)


  def test_620(self):
    DBG.write("test_620", "", verbose)

    # create from scratch, not loadStrPy or loadStrJson
    b = DP4A.DataP4A()
    b["FirstItem"] = 11
    b.SecondItem = 22.
    DBG.write("b", b, verbose)
    DBG.write("b.dumpStrJson()", b.dumpStrJson(), verbose)
    b.ThirdItem = 22.
    DBG.write("b.dumpStrJson()", b.dumpStrJson(), verbose)

    # arithmetic
    self.assertEqual(b.SecondItem, b.ThirdItem)
    self.assertEqual(b.ThirdItem, b.SecondItem)
    self.assertNotEqual(b.FirstItem, b.ThirdItem)
    self.assertNotEqual(b.ThirdItem, b.FirstItem)

    self.assertEqual(b.SecondItem, 22)
    self.assertEqual(22, b.SecondItem)
    self.assertNotEqual(b.FirstItem, 4)
    self.assertNotEqual(4, b.FirstItem)

    b.ThirdItem = 22 # set integer
    DBG.write("b.dumpStrJson()", b.dumpStrJson(), verbose)
    # arithmetic
    self.assertEqual(b.SecondItem, b.ThirdItem)
    self.assertEqual(b.ThirdItem, b.SecondItem)
    self.assertNotEqual(b.FirstItem, b.ThirdItem)
    self.assertNotEqual(b.ThirdItem, b.FirstItem)

    self.assertEqual(b.SecondItem, 22.)
    self.assertEqual(22., b.SecondItem)
    self.assertNotEqual(b.FirstItem, 4)
    self.assertNotEqual(4, b.FirstItem)

  def test_630(self):

    DBG.write("test_630", "", verbose)

    # create from scratch, not loadStrPy or loadStrJson
    b = DP4A.DataP4A()

    import py4amitex.loggerpy.loggingSimple as LOG
    logger = LOG.getDefaultLogger()
    oldlevel = logger.getEffectiveLevel()
    logger.setLevel("CRITICAL")
    self.assertRaises(Exception, b.__getattr__, 'FirstItem') # inexisting, log error not seen
    logger.setLevel(oldlevel)

    self.assertRaises(Exception, b.__getitem__, 'FirstItem') # inexisting, no log error

    b.FirstItem = 11
    b.SecondItem = 22.

    # That is useful: almost as all python casting arithmetics on DataP4A
    b.ThirdItem = b.FirstItem + True
    self.assertEqual(b.ThirdItem, 12)
    b.ThirdItem = b.FirstItem + False
    self.assertEqual(b.ThirdItem, 11)

    b.ThirdItem = b.FirstItem * b.SecondItem
    self.assertEqual(b.ThirdItem.__class__, DP4A.DataP4AFloat)
    self.assertEqual(b.ThirdItem, 242.)

    b.ThirdItem = (b.FirstItem * b.SecondItem == 242.)
    DBG.write("b.dumpStrJson()", b.dumpStrJson(), verbose)
    self.assertTrue(b.ThirdItem)
    self.assertEqual(b.ThirdItem, True)



  def test_999(self):
    # one shot tearDown() for this TestCase
    return


if __name__ == '__main__':
  # verbose = True
  # verbose1 = True
  unittest.main(exit=False)
  pass

