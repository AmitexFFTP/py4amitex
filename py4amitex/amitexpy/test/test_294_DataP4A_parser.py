#!/usr/bin/env python
# -*- coding:utf-8 -*-


import os
import sys
import unittest
import pprint as PP
import math
import numpy as np
import argparse as AP

import py4amitex.debugpy.debug as DBG
import py4amitex.amitexpy.DataP4A as DP4A
import py4amitex.amitexpy.parserP4A as PP4A

verbose = False # False as production, set True if debug unittest
verbosed = True # False as production, set True if debug unittest


class TestCase(unittest.TestCase):
  """Test the parserP4A.ArgumentParserNoExit"""


  def test_000(self):
    # one shot setUp() for this TestCase
    # DBG.write("assert unittest", [a for a in dir(self) if "assert" in a], verbosed)
    pass

  def test_010(self):
    aParser = PP4A.get_common_parser()
    options = aParser.parse_args([])
    DBG.write("options", aParser.getNamespaceStr(options), verbose)
    self.assertEqual(options.cmd, "NONE")
    self.assertEqual(options.doc, False)
    self.assertEqual(options.help, False)
    self.assertEqual(options.verbose, 'INFO')
    self.assertEqual(type(options.workdir), str)
    self.assertTrue(options.workdir is not "")
    self.assertEqual(AP.Namespace, options.__class__)

    options = aParser.parse_args(["-h", "-v=debug"])
    DBG.write("options", aParser.getNamespaceStr(options), verbose)
    self.assertEqual(options.help, True)
    self.assertEqual(options.verbose, "DEBUG")


  def test_020(self):
    aParser = PP4A.get_common_parser()
    options = aParser.parse_args(['--cmd', "bug"])
    DBG.write("options", aParser.getNamespaceStr(options), verbose)
    self.assertEqual(options.cmd, "bug")

    # options = aParser.parse_args(['-v', "oops"])
    self.assertRaises(Exception, aParser.parse_args, ['-v', "oops"])
    # self.assertRaises(Exception, aParser.parse_args, ['-t']) # avoid log critical

  def test_030(self):
    aParser = PP4A.get_common_parser()
    options = aParser.parse_args(['--cmd', "test", "-v", "warning"])
    DBG.write("options", aParser.getNamespaceStr(options), verbose)
    self.assertEqual(options.cmd, "test")

    # cast from result parser AP.Namespace
    optData = DP4A.DataP4A(options)
    DBG.write("options", optData.dumpStrJson(), verbose)

    options.verbose = "ooops" # no control any more
    self.assertEqual(options.verbose, "ooops")
    self.assertEqual(optData.verbose, 'WARNING') # deep copy

    optData.verbose = "OOOOOPS" # no control any more
    self.assertEqual(optData.verbose, "OOOOOPS")
    dump = optData.dumpStrJson()
    DBG.write("dump", dump, verbose)
    expected = '''
  {
    "cmd": "test",
    "verbose": "OOOOOPS",
    "workdir": "/home/wambeke/PY4AMITEX_WORKDIR",
    "help": false,
    "doc": false
  }
    '''
    self.assertTrue('"cmd": "test",' in dump)
    self.assertTrue('"verbose": "OOOOOPS",' in dump)
    self.assertTrue('"workdir": ' in dump)
    self.assertTrue('"help": false,' in dump)
    self.assertTrue('"doc": false' in dump)


  def test_040(self):
    aParser = PP4A.get_common_parser()
    options = aParser.parse_args(['--cmd', "test", "-v", "warning"])

    options.oops = []  # no control any more

    # cast from result parser AP.Namespace
    optData = DP4A.DataP4A(options)
    dump = optData.dumpStrJson()
    DBG.write("options with oops", dump, verbose)
    expected = '''
    {
      "cmd": "test",
      "verbose": "WARNING",
      "workdir": "/home/wambeke/PY4AMITEX_WORKDIR",
      "help": false,
      "doc": false,
      "oops": []
    }
      '''
    self.assertTrue('"oops": []' in dump)

    options.oops = np.zeros(30)  # no control any more

    import py4amitex.loggerpy.loggingSimple as LOG
    logger = LOG.getDefaultLogger()
    self.assertEqual(logger.__class__, LOG.SimpleLogger)
    oldlevel = logger.getEffectiveLevel()

    # logger.setLevel("CRITICAL")
    logger.setHandlersLevel("CRITICAL")
    # avoid [WARNING ] casting user value, hope you know what you are doing with <class 'numpy.ndarray'>
    # avoid [ERROR   ] Unexpected leaf data type <class 'numpy.ndarray'> in DataP4AEncoder, casted to list.

    # cast from result parser AP.Namespace
    optData = DP4A.DataP4A(options)
    dump = optData.dumpStrJson()
    DBG.write("options with oops", dump, verbose)
    expected = '''
  {
    "cmd": "test",
    "verbose": "WARNING",
    "workdir": "/home/wambeke/PY4AMITEX_WORKDIR",
    "help": false,
    "doc": false,
    "oops": []
  }
    '''
    self.assertTrue("ndarray(30,)" in dump)
    # logger.setLevel(oldlevel)
    logger.setHandlersLevel(oldlevel)
    DBG.write("logger level", logger.getEffectiveLevel(), verbose)

  def test_050(self):
    # example of schema test on AP.parser options
    _schema_1 = '''{ 
  "type": "object", 
  "required": [ "cmd", "verbose", "workdir", "help", "doc" ],
  "properties": {
    "cmd": {
      "description": "The command name",
      "type": "string"
    }
  }
}
'''
    aParser = PP4A.get_common_parser()
    options = aParser.parse_args([])
    optData = DP4A.DataP4A(options)
    dump = optData.dumpStrJson()
    DBG.write("options", dump, verbose)
    expected = '''{
  "cmd": "NONE",
  "verbose": "INFO",
  "workdir": "/home/wambeke/PY4AMITEX_WORKDIR",
  "help": false,
  "doc": false
}
'''
    ok = optData.validate(_schema_1)
    self.assertTrue(ok)

    _schema_1 = '''{ 
      "type": "object", 
      "required": [ "cmd", "verbose", "workdir", "help", "doc" ],
      "properties": {
        "cmd": {
          "description": "The command name",
          "type": "integer"
        }
      }
    }
    '''

    import py4amitex.loggerpy.loggingSimple as LOG
    logger = LOG.getDefaultLogger()
    oldlevel = logger.getEffectiveLevel()
    logger.setLevel("CRITICAL")
    ok = optData.validate(_schema_1, verbose=True)
    self.assertFalse(ok)

    _schema_1 = '''{ 
  "type": "object", 
  "required": [ "other" ]
}
'''
    ok = optData.validate(_schema_1, verbose=True)
    self.assertFalse(ok)

    logger.setLevel(oldlevel)


def test_999(self):
    # one shot tearDown() for this TestCase
    return


if __name__ == '__main__':
  # verbose = True
  unittest.main(exit=False)
  pass

