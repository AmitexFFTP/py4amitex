#!/usr/bin/env python
# -*- coding:utf-8 -*-


"""
cd /volatile2/christian/py4amitex
./py4amitex/amitexpy/test/test_290_README_DataP4A.py
"""

import os
import sys
import unittest
import pprint as PP
import json
import numpy as np
import pandas as pd

import py4amitex.amitexpy.DataP4A as DP4A

verbose = False # False as production, set True if debug unittest
verbosed = True # always True for unconditionals prints

# simple dict
data_1_json = r'''
{
  "name": "Tintin",
  "job": "reporter"
}
'''


# dict contains dict
data_2_json = r'''
{
  "heroes": [
    { "name": "Tintin", "job": "reporter", "age": 30 },
    { "name": "Haddock", "job": "capitaine", "age": 60 }
  ]
}
'''


def my_print(*args):
  if verbose: print("->", *args)


###########################################
class TestCase(unittest.TestCase):
  """Test the elementaries principles of dict list inheritance for DataP4A"""

  def test_000(self):
    # one shot setUp() for this TestCase
    # DBG.write("assert unittest", [a for a in dir(self) if "assert" in a], verbose)
    pass

  def test_010(self):
    mes_data = DP4A.DataP4A()
    my_print('mes_data =', mes_data)

  def test_020(self):
    mes_data = DP4A.DataP4A()
    mes_data.loadStrJson(data_1_json)
    my_print('mes_data =', mes_data)
    my_print(mes_data['name'])
    my_print(mes_data['job'])
    my_print(mes_data.name)
    my_print(mes_data.job)

  def test_025(self):
    mes_data = DP4A.DataP4A()
    mes_data.loadStrJson(data_2_json)
    my_print('mes_data =\n%s' % mes_data.dumpStrJson())

    # un peu de parité, c'est difficile avec hergé
    une_femme = {"name": "castafiore", "job": "cantatrice", "age": None}
    mes_data.heroes.append(une_femme)

    haddock = mes_data.heroes[1]
    haddock.name = "Karpock"
    haddock.age -= 5
    my_print('mes_data =\n%s' % mes_data.dumpStrJson())


  def test_030(self):
    mes_data = DP4A.DataP4A()
    mes_data.loadStrJson(data_2_json)
    nb = 1000
    mes_data.some_arrays = DP4A.DataP4A()
    mes_data.some_arrays.array_one = np.ones((nb, nb))

    aDataFrame = pd.DataFrame({'AA': [11, 22, 33], 'BB': [44, 55., 66]})
    mes_data.some_arrays.array_two = pd.DataFrame(aDataFrame)

    # dumpStrJson() resume arrays...
    my_print('mes_data =\n%s' % mes_data.dumpStrJson())

    # dumpFileHdf5() do not resume arrays...
    mes_data.dumpFileHdf5("data_2_modified.hdf5", compression=None)
    mes_data.dumpFileHdf5("data_2_modified_compressed.hdf5", compression='gzip')

    if verbose: os.system("ls -alt data_2_modified*.hdf5")

    mes_data_modified = DP4A.DataP4A()
    mes_data.loadFileHdf5("data_2_modified.hdf5", verbose=verbose)


  def test_050(self):
    schema_1 = '''
{ 
  "type": "object", 
  "required": [ "heroes" ]
}
'''
    schema_2 = '''
{ 
  "type": "object", 
  "required": [ "heroes", "some_arrays" ]
}
'''

    schema_validation = json.loads(schema_1)
    data_to_test = DP4A.DataP4A()
    data_to_test.loadStrJson(data_2_json)
    data_to_test.json_validate(schema_validation)

    schema_validation = json.loads(schema_2)
    try:
      data_to_test.json_validate(schema_validation)
    except Exception as e:
      my_print('mes_data.validate =\n%s' % e)


  def test_100(self):
    mes_data = DP4A.DataP4A()
    mes_data.loadStrJson(data_2_json)
    mes_data.some_arrays = DP4A.DataP4A()
    nb=10
    mes_data.some_arrays.array_one = np.ones((nb, nb))

    for key, value in mes_data.items():
      my_print(key)

    for hero in mes_data.heroes:
      my_print(hero.name)

    a_hero = mes_data.heroes[1]
    a_pythonpath = a_hero.getPythonPath()
    my_print("a_pythonpath = %s" % a_pythonpath)



  def test_200(self):
    dataPy_1 = r'''
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
    mes_data = DP4A.DataP4A()
    mes_data.loadStrPy(dataPy_1)
    my_print("mes_data output json\n%s" % mes_data.dumpStrJson())
    my_print("mes_data output python\n%s" % mes_data.dumpStrPy())




  def test_999(self):
    # one shot tearDown() for this TestCase
    return


if __name__ == '__main__':
  verbose = True
  unittest.main(exit=False)
  pass

