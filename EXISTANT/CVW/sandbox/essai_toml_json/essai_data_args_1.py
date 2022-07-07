#!/usr/bin/env python
# -*- coding: utf-8 -*-

r"""
just tries for simple data input light as dict/list python code
with future compatible argparse
This is ONE of many solutions (JSON etc)
this is forbidden use on \n in strings, why not ?
this is forbidden use of # comment dommage
better use exec compile etc see essai_data_args_2.py
"""

import itertools
from copy import deepcopy
import pprint as PP

##########################################
def myprint(tit, obj):
  print("\n********** ppformat %s %s **********\n\n%s" % (tit, type(obj), PP.pformat(obj)))

##########################################
class DataInput(object):

  def __init__(self, data=None, pattern=None):
    self.pattern = pattern
    self.data = None
    if data is not None:
      self.load(data)

  def load(self, data_str):
    """exec python string code with result as DATA_IN"""
    aDict = {}
    try:
      data_exec = 'DATA_IN = ' + data_str.replace('\n', ' ')
      exec(data_exec, aDict)
    except Exception as e:
      print("ERROR: in %r" % data_str)
      raise Exception(e)
    self.data = aDict['DATA_IN']
    return aDict['DATA_IN']

  def __str__(self):
    return "DataInput(\n%s\n)\n" % PP.pformat(self.data_in)

  

def essai_1():
  myData = DataInput()
  myData.load('["foo", {"bar":["baz", None, 1.0, 2]}]')
  myprint('essai_1', myData.data)
  
def essai_2():
  myData = DataInput()
  myData.load('''
[
  "foo", 
  {
    "bar":
    [
      r"baz", 
      None, 
      1.0, 
      2
    ],
    "range1": [float(i) for i in range(1,10)],
  }
]
''')
  myprint('essai_2', myData.data)
  



if __name__ == "__main__":
  # myprint("dir(DataInput())", dir(DataInput()))
  essai_1()
  essai_2()

