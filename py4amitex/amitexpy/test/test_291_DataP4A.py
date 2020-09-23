#!/usr/bin/env python
# -*- coding:utf-8 -*-


import os
import sys
import unittest
import pprint as PP
import math

import py4amitex.debugpy.debug as DBG
import py4amitex.amitexpy.DataP4A as DP4A

verbose = False # False as production, set True if debug unittest
verbose1 = False # False as production, set True if debug unittest

_dataPy_1 = '''
# you have to KISS, it is DATA in python dict syntax, not program
import math
temp_value = math.pi * 2.
# in dict duplicate keys select last one without warning
DATA_IN = {
    "pi": math.pi,
    "pi2": temp_value,         # comment accepted...
    "nothing": None,
    "abool": True,
    "long_line": """hello
... one more line from hello ...
""",
    "calculated_value": 1+2.,  # all python expressions accepted...
}
'''

# new lines IN PYTHON STRING for json syntax, user HAVE TO USE  --> r''' <---
_dataJson_1 = r'''
{
  "pi": 3.141592653589793,
  "pi2": 6.283185307179586,
  "nothing": null,
  "abool": true,
  "long_line": "hello\n... one more line from hello ...\n",
  "calculated_value": 3.0
}
'''

_keys_dataPy_1 = "pi pi2 nothing long_line calculated_value". split()



class TestCase(unittest.TestCase):
  """Test the DataP4A.py, flat dict, no recursive implementation"""

  def test_000(self):
    # one shot setUp() for this TestCase
    # DBG.write("assert unittest", [a for a in dir(self) if "assert" in a], verbose)
    DBG.write("test_000", "", verbose1)
    import py4amitex.loggerpy.loggingSimple as LOG
    logger = LOG.getDefaultLogger()
    logger.setHandlersLevel("INFO")
    pass

  def test_005(self):
    DBG.write("test_005", "", verbose1)

    # almost immutable need contructor __init__ with mandatory value
    self.assertRaises(Exception, DP4A.DataP4ALeaf)

    # NO cast None and bool as not derivables class, sorry
    self.assertRaises(Exception, DP4A.DataP4ALeaf, None)
    self.assertRaises(Exception, DP4A.DataP4ALeaf, True)
    self.assertRaises(Exception, DP4A.DataP4ALeaf, False)

    # almost immutable for a._value
    # as a._value = 'othernew'
    a = DP4A.DataP4ALeaf(object())
    self.assertRaises(Exception, a.__setattr__, '_value', 'othernew')

    a = DP4A.DataP4A({'hello': 123})
    a = DP4A.DataP4A({})
    a = DP4A.DataP4A()
    #a = DP4A.DataP4A(12)
    #DBG.write("a 12", a, True)

    self.assertRaises(Exception, DP4A.DataP4A, [])  # as *arg empty list is same as no arg
    self.assertRaises(Exception, DP4A.DataP4A, 12)
    self.assertRaises(Exception, DP4A.DataP4A, [123])


    #self.assertEqual(a, {})

    for i in [0, 1, -1]:
      a = DP4A.DataP4AInt(i)
      self.assertEqual(a, i)
      self.assertRaises(Exception, DP4A.DataP4ALeaf, i)
      self.assertRaises(Exception, DP4A.DataP4ADict, i)
      self.assertRaises(Exception, DP4A.DataP4AList, i)

    for i in [0., 1.1, -1.1]:
      a = DP4A.DataP4AFloat(i)
      self.assertEqual(a, i)
      self.assertRaises(Exception, DP4A.DataP4ALeaf, i)
      self.assertRaises(Exception, DP4A.DataP4ADict, i)
      self.assertRaises(Exception, DP4A.DataP4AList, i)

    for i in [{}, {'hello': 123}]:
      # print('\nDataP4ADict init by', i)
      a = DP4A.DataP4ADict(i)
      self.assertEqual(a, i)
      # print('\nDataP4ALeaf exception init by', i)
      self.assertRaises(Exception, DP4A.DataP4ALeaf, i)
      self.assertRaises(Exception, DP4A.DataP4AList, i)

    for i in [[], ['hello', 'bye']]:
      a = DP4A.DataP4AList(i)
      self.assertEqual(a, i)
      self.assertRaises(Exception, DP4A.DataP4ALeaf, i)
      self.assertRaises(Exception, DP4A.DataP4ADict, i)

    for i in [{}, {'hello': {}}]:
      # print('\nDataP4A init by', i)
      a = DP4A.DataP4A(i)
      self.assertEqual(a, i)
      self.assertRaises(Exception, DP4A.DataP4ALeaf, i)
      self.assertRaises(Exception, DP4A.DataP4AList, i)

    for i in [[], ['hello', 'bye']]: # DataP4A is DataP4ADict
      self.assertRaises(Exception, DP4A.DataP4A, i)
      # DBG.write("a", str(a), verbose1)



  def test_010(self):
    DBG.write("test_010", "", verbose1)
    a = DP4A.DataP4A()
    self.assertEqual(a, {})
    self.assertEqual(a._parent, None)

  ### loadPy use python syntax for data, no limits!
  ### avoid if then else please: it is DATA, not program
  def test_020(self):
    DBG.write("test_020", "", verbose1)
    a = DP4A.DataP4A()
    dataPy = '''DATA_IN = {"name": "Haddock", "job": "capitaine"}'''
    a.loadPy(dataPy, verbose)
    # PrettyPrinter works well if you don't care about the order of the keys
    aStr = a.__repr__()
    DBG.write("a.__repr__()", aStr, verbose)
    self.assertTrue("'job':" in aStr)
    self.assertTrue("'name':" in aStr)
    self.assertTrue("'capitaine'" in aStr)
    self.assertTrue("'Haddock'" in aStr)


  def test_022(self):
    DBG.write("test_022", "", verbose1)
    a = DP4A.DataP4A()
    a.loadPy(_dataPy_1)
    DBG.write("a", a, verbose)
    aStr = a.__repr__()
    DBG.write("a.__repr__()", aStr, verbose)
    self.assertTrue("'pi':" in aStr)
    self.assertTrue("'pi2':" in aStr)
    self.assertTrue("'nothing':" in aStr)
    self.assertTrue(r"'long_line':" in aStr)
    self.assertTrue("'calculated_value':" in aStr)
    self.assertTrue("3.14159" in aStr)
    self.assertTrue("6.28318" in aStr)
    self.assertTrue("None" in aStr)
    self.assertTrue(r"'hello\n... one more line from hello ...\n'" in aStr)
    self.assertTrue("3.0" in aStr)


  ### dumpJson
  def test_024(self):
    DBG.write("test_024", "", verbose1)
    a = DP4A.DataP4A()
    a.loadPy(_dataPy_1)
    DBG.write("a", a, verbose)
    aStr = a.dumpJson()
    DBG.write("a.dumpJson()", aStr, verbose)
    self.assertTrue('"pi": 3.14159' in aStr)
    self.assertTrue('"pi2": 6.28318' in aStr)
    self.assertTrue('"nothing": null' in aStr)
    self.assertTrue(r'"long_line": "hello\n... one more line from hello ...\n"' in aStr)
    self.assertTrue('"calculated_value": 3.0' in aStr)


  ### loadJson
  def test_040(self):
    DBG.write("test_040", "", verbose1)
    a = DP4A.DataP4A()
    a.loadJson('{"name": "Haddock", "job": "capitaine"}')
    aStr = a.dumpJson()
    DBG.write("a.dumpJson()", aStr, verbose)
    self.assertTrue('"name": "Haddock"' in aStr)
    self.assertTrue('"job": "capitaine"' in aStr)

    # keys simple cote forbidden
    self.assertRaises(Exception, a.loadJson, "{'name': 'Haddock'}", verbose=False)


  def test_042(self):
    DBG.write("test_042", "", verbose1)
    a = DP4A.DataP4A()
    DBG.write("_dataJson_1", _dataJson_1, verbose)
    a.loadJson(_dataJson_1)
    aStr = a.__repr__()
    DBG.write("a.__repr__()", aStr, verbose)
    for i, v  in a.items():
      DBG.write("i.__repr__()", (type(v), i, v), verbose)

    aStr = a.dumpJson()
    DBG.write("a.dumpJson()", aStr, verbose)
    self.assertTrue('"pi": 3.14159' in aStr)
    self.assertTrue('"pi2": 6.28318' in aStr)
    self.assertTrue('"nothing": null' in aStr)
    self.assertTrue(r'"long_line": "hello\n... one more line from hello ...\n"' in aStr)
    self.assertTrue('"calculated_value": 3.0' in aStr)


  ### dict["key"] coding way: a._data["pi"]
  def test_122(self):
    DBG.write("test_122", "", verbose1)
    a = DP4A.DataP4A()
    a.loadPy(_dataPy_1)
    DBG.write("a", a, verbose)
    for k in _keys_dataPy_1:
      self.assertTrue(k in a)
      value = a[k]
    self.assertEqual(math.pi, a.pi)
    self.assertEqual(math.pi, a["pi"])


  ### class.key coding way: a.pi
  def test_222(self):
    DBG.write("test_222", "", verbose1)
    a = DP4A.DataP4A()
    a.loadPy(_dataPy_1)
    self.assertEqual(math.pi, a.pi)
    self.assertEqual(math.pi*2, a.pi2)
    self.assertEqual(None, a.nothing)
    self.assertEqual(3., a.calculated_value)


  ### traps and tricks
  def test_300(self):
    DBG.write("test_300", "", verbose1)
    #"""traps and tricks"""
    a = DP4A.DataP4A()
    dataPy = ('''DATA_IN = {"trick": "with l'apostrophe!"}''')
    a.loadPy(dataPy)
    aStr = a.__repr__()
    DBG.write("a.__repr__()", aStr, verbose)
    self.assertTrue("'trick':" in aStr)
    self.assertTrue('''"with l'apostrophe!"''' in aStr)

    dataPy = ('''DATA_IN = {0: "indice 0"}''')
    # as a.loadPy(dataPy)
    self.assertRaises(Exception, a.loadPy, dataPy, False) # False as not verbose


  ### raises Exceptions
  def test_350(self):
    DBG.write("test_350", "", verbose1)
    a = DP4A.DataP4A()
    dataPy = ('''DATA_IN = oops # it is a bug!''')
    # a logger [CRITICAL] message is skipped (by loadPy verbose=False)
    self.assertRaises(Exception, a.loadPy, dataPy, False)


  def test_999(self):
    # one shot tearDown() for this TestCase
    return


if __name__ == '__main__':
  # verbose1 = True
  unittest.main(exit=False)
  pass

