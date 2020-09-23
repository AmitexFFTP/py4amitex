#!/usr/bin/env python
#-*- coding:utf-8 -*-


import os
import sys
import unittest
import pprint as PP

import py4amitex.debugpy.debug as DBG
import py4amitex.amitexpy.ApiP4A as AP4A

verbose = False # True

class TestCase(unittest.TestCase):
  "Test the amitexpy.py"""
  
  def test_000(self):
    # one shot setUp() for this TestCase
    if verbose:
      DBG.push_debug(True)
      # list all assertxxx current methods
      DBG.write("assert unittest", [a for a in dir(self) if "assert" in a])
    pass
  
  def test_010(self):
    a = AP4A.RunnerP4A()
    self.assertEqual(a.args, None)
    self.assertEqual(a.options, None)

  def test_020(self):
    a = AP4A.RunnerP4A()
    self.assertNotEqual(a.parser, None)
    self.assertIn("usage", a.get_help())
    options = a.parseArguments([])
    self.assertEqual(options.help, False)

  def test_999(self):
    # one shot tearDown() for this TestCase
    if verbose:
      DBG.pop_debug()
    return
    
if __name__ == '__main__':
    unittest.main(exit=False)
    pass

