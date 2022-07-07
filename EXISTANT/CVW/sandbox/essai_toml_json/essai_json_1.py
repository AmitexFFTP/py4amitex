#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
pip install json
"""

import itertools
import json
from copy import deepcopy
import pprint as PP

def myprint(tit, obj):
  print("\n********** ppformat %s %s **********\n\n%s" % (tit, type(obj), PP.pformat(obj)))

def jsonprint(tit, obj):
  print("\n********** jsondumps %s %s **********\n\n%s" % (tit, type(obj), json.dumps(obj)))

def essai_1():
  obj = json.loads('["foo", {"bar":["baz", null, 1.0, 2]}]')
  myprint('json 1', obj)
  jsonprint('json 1', obj)

def essai_2():  
  obj = json.loads('''
{
  "bar":
  [
    "baz", 
    null, 
    1.0, 
    2
  ]
}
''')

  myprint('json 2', obj)
  jsonprint('json 2', obj)


if __name__ == "__main__":
  myprint("dir(json)", dir(json))
  essai_1()
  essai_2()

