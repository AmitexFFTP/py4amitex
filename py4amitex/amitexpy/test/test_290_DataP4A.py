#!/usr/bin/env python
# -*- coding:utf-8 -*-


import os
import sys
import unittest
import pprint as PP
import json

import py4amitex.debugpy.debug as DBG

verbose = False # False as production, set True if debug unittest
verbose1 = False # True # False as production, set True if debug unittest

# simple dict
_json_1 = '''{"name": "Haddock", "job": "capitaine"}'''

# dict contains dict
_json_2 = r'''
{
  "heroes": {
    "Tintin": { "job": "reporter", "age": 30 },
    "Haddock": { "job": "capitaine", "age": 60 }
  }
}
'''


###########################################
class DictLikeClass(object):
  """
  You can use like a dict whit attributes names as dict keys

  >> d = DictLikeClass()
  >> d["key"] = "value"  # as dict
  >> print(d["key"])     # as normal dict
  >> print(d.key)        # as attribute (shorter)
  """
  def __init__(self, *arg, **kw):
    super().__init__(*arg, **kw)

  def __getitem__(self, key):
    return getattr(self, key)

  def __setitem__(self, key, value):
    setattr(self, key, value)

###########################################
class dictWithAttr(dict):
  def __init__(self, *arg, **kw):
    self.toto = 1
    super().__init__(*arg, **kw)

###########################################
class EssaiList(list):
  def __init__(self, *arg, **kw):
    self._parent = None
    super().__init__(*arg, **kw)


###########################################
class EssaiDict_0(dict):
  def __init__(self, *arg, **kw):
    self._setitemdone = False
    super().__init__(*arg, **kw)
    # here because constructor dict from arg do not use __setitem__
    # TODO something...
    # PP.pprint(dir(self))

  def __setitem__(self, key, value):
    # print("EssaiDict_0.__setitem__", key, value)
    self._setitemdone = True
    return super().__setitem__(key, value)

  def update(self, aDict):
    # print("EssaiDict_0.update", aDict)
    return super().update(aDict)



###########################################
class EssaiDict(dict):

  def __init__(self, *arg, **kw):
    self._parent = None
    super().__init__(*arg, **kw)
    DBG.write('EssaiDict.__init__', (arg, kw), verbose)
    # here because constructor dict from arg do not use __setitem__
    if True:
      for key, v in self.items():
        # print("%%%", k, v, v.__class__, id(v))
        if v.__class__ == EssaiDict:
          DBG.write("ok %s parent of %s key %r" % (id(self), id(v), key), "", verbose)
          if v._parent is not None:
            raise Exception("EssaiDict constructor dict key %r mandatory have _parent None" % key)
          v._parent = self
          #DBG.write("v.__repr_with_parent__()", v.__repr_with_parent__(), verbose1 )

  def __setitem__(self, key, value):
    if not issubclass(key.__class__, str):
      raise Exception("EssaiDict key %r mandatory have to be str" % key)
    DBG.write("EssaiDict.__setitem__ %r <- %r" % (key, value), "", verbose)
    if hasattr(value, "_parent"):
      if value._parent is not None:
        raise Exception("EssaiDict __setitem__ dict key %r value mandatory have _parent None\n%s" % (key, value))
    try:
      value._parent = self
    except Exception as e:
      # DBG.write("Exception", e, verbose)
      DBG.write("have to be immutable value %r" % value, "", verbose)
      pass
    return super().__setitem__(key, value)

  def __repr__(self):
    return "EssaiDict(" + super().__repr__() + ")"

  def __getattr__(self, attr):
    DBG.write("EssaiDict.__getattr__ %r" % attr, "", verbose)
    if "_" != attr[0]:
      return self[attr]
    else:
      DBG.write("EssaiDict.__getattr__ classical %r" % attr, "", verbose)
      return super().__getattr__(attr)

  def __repr_with_parent__(self):
    """to debug parent tree functionality"""
    return "EssaiDict(\n_parent=%s,\nself=%s\n)" % (self._parent, super().__repr__()) + ")"


###########################################
class TestCase(unittest.TestCase):
  """Test the elementaries principles of dict list inheritance for DataP4A"""

  def test_000(self):
    # one shot setUp() for this TestCase
    # DBG.write("assert unittest", [a for a in dir(self) if "assert" in a], verbose)
    pass

  def test_004(self):
    DBG.write("test_004", "", verbose1)
    a = EssaiDict_0()
    self.assertFalse(a._setitemdone)
    a["key1"] = "value by setitem"
    self.assertTrue(a._setitemdone)
    self.assertEqual("value by setitem", a["key1"])
    a = EssaiDict_0({"key2": "value by init"}) # do not call __setitem___ !
    self.assertFalse(a._setitemdone) # proof have not call __setitem___ !
    a["key3"] = "value by setitem"
    self.assertEqual("value by init", a["key2"])
    self.assertEqual("value by setitem", a["key3"])
    self.assertTrue(a._setitemdone) # proof have not call __setitem___ !

  def test_005(self):
    DBG.write("test_005", "", verbose1)
    a = DictLikeClass()
    a["key"] = "value"
    self.assertEqual("value", a["key"])
    self.assertEqual("value", a.key)
    self.assertEqual(id(a["key"]), id(a.key))

  ### what could we do with inherited class 'list' ?
  # this is experience for DataP4A class which could be a dict or a list
  # But could be cool in Json usage (only dict and list, in tree)
  def test_010(self):
    DBG.write("test_010", "", verbose1)
    a = EssaiList()
    a._0_ = "indice _0_ as attribute almost dict" # obviously useless
    a.append("indice 0 as list")

    self.assertEqual("indice 0 as list", a[0])
    self.assertEqual("indice _0_ as attribute almost dict", a._0_)
    self.assertEqual("['indice 0 as list']", str(a))
    self.assertEqual(None, a._parent)

    DBG.write("EssaiList a[0]", a[0], verbose)
    DBG.write("EssaiList a._0_", a._0_, verbose)
    DBG.write("EssaiList str(a)", str(a), verbose)


  ### what could we do with inherited class 'dict' ?
  # this is experience for DataP4A class which could be a dict or a list
  # But could be cool in Json usage (only dict and list, in tree)
  def test_110(self):
    DBG.write("test_110", "", verbose1)
    a = EssaiDict()
    self.assertEqual(None, a._parent)

    # a[0] = "oops"  # obviously tricky useless as supposed JSON keys are str
    self.assertRaises(Exception, a.__setitem__, 0, "oops")

    a["hello"] = "hello from a"
    self.assertEqual("hello from a", a["hello"])
    self.assertEqual("EssaiDict({'hello': 'hello from a'})", str(a))
    self.assertEqual(None, a._parent)

    DBG.write("EssaiDict str(a)", str(a), verbose)
    DBG.write("EssaiDict dir(Essai as dict)", dir(a), verbose)

    b = EssaiDict()
    b['aaaaa'] = a
    self.assertEqual(b, a._parent)
    DBG.write("EssaiDict str(b)", str(b), verbose)
    DBG.write("EssaiDict a__repr_with_parent__()", a.__repr_with_parent__(), verbose)

  def test_510(self):
    DBG.write("test_510", "", verbose1)
    a = json.loads(_json_1)
    self.assertEqual("{'name': 'Haddock', 'job': 'capitaine'}", str(a))
    self.assertEqual('{"name": "Haddock", "job": "capitaine"}', json.dumps(a))
    DBG.write("_json_1", json.dumps(a), verbose)

  # dict contains dict
  def test_512(self):
    DBG.write("test_512", "", verbose1)
    a = json.loads(_json_2)
    self.assertEqual("{'heroes': {'Tintin': {'job': 'reporter', 'age': 30}, 'Haddock': {'job': 'capitaine', 'age': 60}}}", str(a))
    DBG.write("_json_2", json.dumps(a, indent=2), verbose)
    DBG.write("_json_2", str(a), verbose)

  # tries with EssaiDict
  def test_515(self):
    DBG.write("test_515 tries with EssaiDict", "", verbose1)
    a = json.loads(_json_1, object_pairs_hook=EssaiDict)
    self.assertEqual(EssaiDict, a.__class__)
    self.assertEqual("EssaiDict({'name': 'Haddock', 'job': 'capitaine'})", str(a))
    self.assertEqual('{"name": "Haddock", "job": "capitaine"}', json.dumps(a))
    DBG.write("_json_1", json.dumps(a), verbose)
    self.assertEqual(None, a._parent)

  def test_525(self):
    DBG.write("test_525 tries with EssaiDict dict contains dict", "", verbose1)
    a = json.loads(_json_2, object_pairs_hook=EssaiDict)
    DBG.write("_json_2", json.dumps(a, indent=2), verbose)
    self.assertEqual(EssaiDict, a.__class__)
    self.assertEqual(EssaiDict, a["heroes"].__class__)
    self.assertEqual(EssaiDict, a["heroes"]["Tintin"].__class__)
    self.assertEqual(EssaiDict, a["heroes"]["Haddock"].__class__)

    self.assertEqual("reporter", a["heroes"]["Tintin"]["job"])
    self.assertEqual(60, a["heroes"]["Haddock"]["age"])

    # tries as attribute syntax
    self.assertEqual("reporter", a.heroes.Tintin.job)
    self.assertEqual(60, a.heroes.Haddock.age)

    # tries as _parent attribute
    DBG.write("id(a)", id(a), verbose)
    DBG.write('id(a["heroes"])', id(a["heroes"]), verbose)
    DBG.write('a["heroes"]', a["heroes"].__repr_with_parent__(), verbose)

    self.assertEqual(None, a._parent)
    self.assertEqual(a, (a["heroes"])._parent)
    self.assertEqual(a["heroes"], (a["heroes"]["Tintin"])._parent)
    self.assertEqual(a["heroes"], (a["heroes"]["Haddock"])._parent)

    self.assertEqual(None, a._parent)
    self.assertEqual(a, a.heroes._parent)
    self.assertEqual(a.heroes, a.heroes.Tintin._parent)
    self.assertEqual(a.heroes, a.heroes.Haddock._parent)

    # no creating loops in tree, thanks to _parent functionality precaution
    DBG.write('a.heroes.Tintin', a.heroes.Tintin.__repr_with_parent__(), verbose)
    self.assertNotEqual(None,  a.heroes._parent) # a.heroes._parent is not None...
    DBG.write("loop_2", json.dumps(a, indent=2), verbose)
    # a.heroes.Tintin["heroesToLoop"] = a.heroes  # obviously create tree loop, raise exception
    # ...so EssaiDict.__setitem__Exception
    self.assertRaises(Exception, a.heroes.Tintin.__setitem__, "heroesToLoop", a.heroes)


  def test_999(self):
    # one shot tearDown() for this TestCase
    return


if __name__ == '__main__':
  # verbose1 = True
  unittest.main(exit=False)
  pass

