#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
py4amitex command launch test(s).
Launch python unittest (short time) and/or integration tests (more long time)
"""

import os
import sys
import pprint as PP

import py4amitex
import py4amitex.debugpy.debug as DBG  # Easy print stderr (for DEBUG only)
import py4amitex.loggerpy.loggingSimple as LOG
from py4amitex.returncodepy.returnCode import ReturnCode
from py4amitex.amitexpy.parserP4A import getNameCmdFromFileName


logger = LOG.getDefaultLogger()

__cmdname__ = getNameCmdFromFileName(__file__) # 'xxx' <- '.../cmd_xxx.py'

here = os.path.realpath(os.path.dirname(__file__))
rootpackagedir = os.path.dirname(os.path.dirname(here))

def add_cmd_parser(parser):
  """
  using --cmd xxx then no need subtil add_subparsers method usage
  this way raise conflicts with previously common arguments set
  theses raises are catched for critical message
  as argparse.ArgumentError: argument ... conflicting option
  """
  parser.description = "command line interface for py4amitex --cmd %s stuff" % __cmdname__
  parser.add_argument(
    '-p', '--pattern',
    help="file pattern for unittest files ['test_*.py'|'*Test.py'...]",
    default="test_???_*.py", # as alphabetical ordered test site
    metavar='filePattern'
  )
  return

def compute(iP4A):
  """
  iP4A is instance of RunnerP4A
  which contains all caller data informations
  """

  # this command begin common stuff
  parser = iP4A.parser # shortcut
  args = iP4A.args # shortcut
  logger.info("<data>Begin %s.compute with args %s<reset>" % (__cmdname__, args))

  # this command more options
  add_cmd_parser(parser)
  logger.info("<header>%s<reset>\n%s" % (__doc__, parser.format_help()))
  options = parser.parse_args(args)
  logger.info("%s options:\n\n<data>%s<reset>\n" %
              (__cmdname__, parser.getNamespaceStr(options)))

  # this command real stuff

  # use AllTestLauncher parser API entry
  import unittestpy.AllTestLauncher as ALLTEST

  test_args = []
  test_parser = ALLTEST.getParser()
  debug = (options.verbose == 'DEBUG')
  if debug: # only for debug
    logger.debug(
      "AllTestLauncher command line usage:\n\n%s" %
      test_parser.format_help()
    )
    test_args.append('--debug')
    # print('dir(test_parser)\n%s' % PP.pformat(dir(test_parser)))

  test_args.append('--rootPath=%s' % rootpackagedir)
  test_args.append('--pattern=%s' % options.pattern)
  test_options = test_parser.parse_args(test_args)

  # directory = os.path.realpath(args.rootPath)
  logger.debug("test_args:\n  %s" % PP.pformat(test_args))
  logger.debug("test_options:\n  %s" % PP.pformat(test_options))

  # sys.path.insert(0, directory) #supposed to be root of a package
  ALLTEST.runOnArgs(test_options)


  logger.info("<data>End %s.compute" % __cmdname__)
  return ReturnCode("OK", "End %s.compute" % __cmdname__)
