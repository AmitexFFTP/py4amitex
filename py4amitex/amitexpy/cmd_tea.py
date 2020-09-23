#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
py4amitex command launch tea.
cmd_tea.py is as pattern for future new commands
developer tips:
- copy file cmd_tea.py as cmd_something.py in amitexpy folder
- modify cmd_something.py
- courage
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

_bag = r"""

            .------.____
         .-'       \ ___)
      .-'         \\\
   .-'        ___  \\)
.-'          /  (\  |)
         __  \  ( | |
        /  \  \__'| |
       /    \____).-'
     .'       /   |
    /     .  /    |
  .'     / \/     |
 /      /   \     |
       /    /    _|_
       \   /    /\ /\
        \ /    /__v__\
         '    |       |
              |     .#|
              |#.  .##|
              |#######|
              |#######|
"""

_tea = r"""

            (  )   (   )  )
             ) (   )  (  (
             ( )  (    ) )
             _____________
            <_____________> ___
            |             |/ _ \
            |               | | |
            |               |_| |
         ___|             |\___/
        /    \___________/    \
        \_____________________/

"""

def add_cmd_parser(parser):
  """
  using --cmd xxx then no need subtil add_subparsers method usage
  this way raise conflicts with previously common arguments set
  theses raises are catched for critical message
  as argparse.ArgumentError: argument ... conflicting option
  """
  parser.description = "command line interface for py4amitex --cmd %s stuff" % __cmdname__
  parser.add_argument(
    '-b', '--bag',
    help='add teabag',
    default=False,
    action='store_true'
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
  msg = r""
  if options.bag is True:
    msg += _bag
  msg += _tea
  logger.info(msg)

  logger.info("<data>End %s.compute" % __cmdname__)
  return ReturnCode("OK", "End %s.compute" % __cmdname__)
