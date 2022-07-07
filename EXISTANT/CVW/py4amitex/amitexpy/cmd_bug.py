#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
py4amitex command launch example of bug.
Developer see here example/test of bug programming in his own command
Error have to be catched (and logged) in ApiP4A.RunnerP4A instance caller
"""

import py4amitex.loggerpy.loggingSimple as LOG

logger = LOG.getDefaultLogger()

def compute(iP4A):
  """
  iP4A is instance of RunnerP4A
  """
  logger.warning('Here comes the bug ... ')
  ooops

  return ReturnCode("OK", "End %s.compute" % __cmdname__)
