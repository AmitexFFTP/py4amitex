#!/usr/bin/env python
# -*- coding:utf-8 -*-


import os
import sys
import unittest
import pprint as PP
import math
import numpy as np
import json

import py4amitex.debugpy.debug as DBG
import py4amitex.amitexpy.DataP4A as DP4A

verbose = False # False as production, set True if debug unittest
verbose1 = False # False as production, set True if debug unittest

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

# see https://json-schema.org/learn/getting-started-step-by-step.html
_schema_1 = '''{ "type": "object" }'''
_schema_2 = '''{ "type": "object", "required": [ "something" ]}'''
_schema_3 = '''{ 
  "type": "object", 
  "required": [ "pi", "nb_heroes", "heroes" ]
}
'''

_schema_4 = '''{ 
  "type": "object", 
  "required": [ "pi", "nb_heroes", "heroes" ],
  "properties": {
    "nb_heroes": {
      "description": "The number of heroes",
      "type": "integer"
    }
  }
}
'''


class TestCase(unittest.TestCase):
  """
  Test the DataP4A.py,
  https://json-schema.org/learn/getting-started-step-by-step.html
  """

  def test_000(self):
    # one shot setUp() for this TestCase
    # DBG.write("assert unittest", [a for a in dir(self) if "assert" in a], verbose1)
    pass

  def test_004(self):
    dict_valid = json.loads('{}')
    dict_to_test = json.loads('{"everything": 123}')
    DP4A.json_validate(dict_to_test, dict_valid)

    dict_valid = json.loads(_schema_1)
    dict_to_test = json.loads('{"everything": 123}')
    DP4A.json_validate(dict_to_test, dict_valid)

    dict_valid = json.loads(_schema_2)
    dict_to_test = json.loads('{"everything": 123}')
    self.assertRaises(Exception, DP4A.json_validate, dict_to_test, dict_valid)

    dict_valid = json.loads(_schema_2)
    dict_to_test = json.loads('{"something": 123}')
    DP4A.json_validate(dict_to_test, dict_valid)

  def test_005(self):
    dict_valid = json.loads(_schema_3)
    dict_to_test = DP4A.DataP4A()
    dict_to_test.loadJson(_dataJson_1)
    DP4A.json_validate(dict_to_test, dict_valid)

    dict_valid = DP4A.DataP4A()
    dict_valid.loadJson(_schema_4)
    DP4A.json_validate(dict_to_test, dict_valid)

    dict_to_test.nb_heroes = "ooops" # NOT integer
    self.assertRaises(Exception, DP4A.json_validate, dict_to_test, dict_valid)


  def test_999(self):
    # one shot tearDown() for this TestCase
    return


if __name__ == '__main__':
  # verbose = True
  verbose1 = True
  unittest.main(exit=False)
  pass

