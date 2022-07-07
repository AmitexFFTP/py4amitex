#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
pip install toml
"""


""" ? as recursive what do you prefer ?

********** ppformat scenarios deepcopy <class 'dict'> **********

{'sc_0': {'initialWealth': 55,
          'maxiter': 40000,
          'nAgents': 111,
          'nReplicates': 100,
          'seed': 314159},
 'sc_1': {'initialWealth': 66,
          'maxiter': 40000,
          'more': {'initialWealth': 55,
                   'maxiter': 40000,
                   'nAgents': 111,
                   'nReplicates': 100,
                   'seed': 314159},
          'nAgents': 111,
          'nReplicates': 100,
          'seed': 314159},
 'sc_2': {'initialWealth': 77,
          'maxiter': 40000,
          'more': {'initialWealth': 66,
                   'maxiter': 40000,
                   'more': {'initialWealth': 55,
                            'maxiter': 40000,
                            'nAgents': 111,
                            'nReplicates': 100,
                            'seed': 314159},
                   'nAgents': 111,
                   'nReplicates': 100,
                   'seed': 314159},
          'nAgents': 111,
          'nReplicates': 100,
          'seed': 314159},
 'sc_3': {'initialWealth': 55,
          'maxiter': 40000,
          'more': {'initialWealth': 77,
                   'maxiter': 40000,
                   'more': {'initialWealth': 66,
                            'maxiter': 40000,
                            'more': {'initialWealth': 55,
                                     'maxiter': 40000,
                                     'nAgents': 111,
                                     'nReplicates': 100,
                                     'seed': 314159},
                            'nAgents': 111,
                            'nReplicates': 100,
                            'seed': 314159},
                   'nAgents': 111,
                   'nReplicates': 100,
                   'seed': 314159},
          'nAgents': 222,
          'nReplicates': 100,
          'seed': 314159}}

********** tomldumps scenarios as dict <class 'dict'> **********

[sc_0]
nAgents = 111
initialWealth = 55
maxiter = 40000
seed = 314159
nReplicates = 100

[sc_1]
nAgents = 111
initialWealth = 66
maxiter = 40000
seed = 314159
nReplicates = 100

[sc_2]
nAgents = 111
initialWealth = 77
maxiter = 40000
seed = 314159
nReplicates = 100

[sc_3]
nAgents = 222
initialWealth = 55
maxiter = 40000
seed = 314159
nReplicates = 100

[sc_1.more]
nAgents = 111
initialWealth = 55
maxiter = 40000
seed = 314159
nReplicates = 100

[sc_2.more]
nAgents = 111
initialWealth = 66
maxiter = 40000
seed = 314159
nReplicates = 100

[sc_3.more]
nAgents = 111
initialWealth = 77
maxiter = 40000
seed = 314159
nReplicates = 100

[sc_2.more.more]
nAgents = 111
initialWealth = 55
maxiter = 40000
seed = 314159
nReplicates = 100

[sc_3.more.more]
nAgents = 111
initialWealth = 66
maxiter = 40000
seed = 314159
nReplicates = 100

[sc_3.more.more.more]
nAgents = 111
initialWealth = 55
maxiter = 40000
seed = 314159
nReplicates = 100

"""

import itertools
import toml
from copy import deepcopy
import pprint as PP

def myprint(tit, obj):
  print("\n********** ppformat %s %s **********\n\n%s" % (tit, type(obj), PP.pformat(obj)))

def tomlprint(tit, obj):
  print("\n********** tomldumps %s %s **********\n\n%s" % (tit, type(obj), toml.dumps(obj)))

def essai_1():
  info = toml.load("xmplBaselineGW.toml")               # parse the file
  baseline = dict((key, fields['value']) for (key, fields) in info.items()) # create baseline
  # myprint("baseline", baseline)
  myprint("info", info)
  tomlprint("info", info)
  return baseline

def essai_2(baseline):
  xpmts = toml.load('xmplExperimentsGW.toml') # parse experiments
  tomlprint("xpmts", xpmts)
  xpmt = xpmts['multiSweep']                  # get an experiment
  names, lists = zip(*xpmt.items())           # separate names & sweep values
  scenarios = list()                          # empty list for scenarios
  for vals in itertools.product(*lists): # for each item in Cartesian product
      new = baseline.copy()              # create a copy of `baseline`
      new.update(zip(names,vals))        # overwrite with scenario values
      scenarios.append(new)              # add new scenario to scenarios
  # myprint("scenarios", scenarios)
  myprint("scenarios", scenarios)
  
  # try to insert sc_2.suite.suite etc.
  adict = {}
  i = 0
  for s in scenarios[0:4]: # limit recurse 6 
    # toml/encoder.py", line 67, in dumps
    # raise ValueError("Circular reference detected")
    if i > 0: 
      s["more"] = sm1
    adict["sc_%i" % i] = deepcopy(s)
    sm1 = deepcopy(s)
    i += 1
  myprint("scenarios deepcopy", adict)
  tomlprint("scenarios as dict", adict)

if __name__ == "__main__":
  myprint("dir(toml)", dir(toml))
  baseline = essai_1()
  essai_2(baseline)
