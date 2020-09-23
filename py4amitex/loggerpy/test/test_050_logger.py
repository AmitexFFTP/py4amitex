#!/usr/bin/env python
# -*- coding:utf-8 -*-


import os
import sys
import unittest

verbose = False

class TestCase(unittest.TestCase):

  def test_000(self):
    # one shot setUp() for this TestCase
    # DBG.write("assert unittest", [a for a in dir(self) if "assert" in a], verbose1)
    pass

  def test_030(self):
    import py4amitex.loggerpy.loggingSimple as LOG
    logger = LOG.getDefaultLogger()
    self.assertEqual(logger.__class__, LOG.SimpleLogger)
    oldlevel = logger.getEffectiveLevel()
    if verbose: # no test without human eyes
      logger.warning("logger level is %s" % oldlevel)
      #logger.setLevel("CRITICAL")
      logger.setHandlersLevel("CRITICAL")
      # only bb
      logger.warning("aa logger level is %s" % logger.getEffectiveLevel())
      logger.critical("bb logger level is %s" % logger.getEffectiveLevel())
      #logger.setLevel(oldlevel)
      logger.setHandlersLevel(oldlevel)
      # cc and dd
      logger.warning("cc logger level is %s" % logger.getEffectiveLevel())
      logger.critical("dd logger level is %s" % logger.getEffectiveLevel())

def test_999(self):
    # one shot tearDown() for this TestCase
    return


if __name__ == '__main__':
  verbose = True
  unittest.main(exit=False)
  pass

