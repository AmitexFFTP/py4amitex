#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
This file contains parser argparse for py4amitex
"""

import os
import sys
import glob
import pprint as PP
import argparse as AP

import py4amitex
import py4amitex.amitexpy.utilsP4A as UP4A
import py4amitex.debugpy.debug as DBG  # Easy print stderr (for DEBUG only)
import py4amitex.loggerpy.loggingSimple as LOG

logger = LOG.getDefaultLogger()

verbose = False

########################################################################
# parser arguments py4amitex
########################################################################

class ArgumentParserNoExit(AP.ArgumentParser):
  """change Exiting method as no exit python interpreter anyway"""

  def exit(self, status=0, message=None):
    if message:
      self._print_message(message, AP._sys.stderr)
    return

  def error(self, message):
    # raise Exception("parse argument: %s" % message)
    logger.critical(message)

  def getLogLevels(self):
    return logger._knownLevels

  def getLogLevelsStr(self):
    return LOG._knownLevelsStr

  #################################################
  # common filter methods
  #################################################

  def filter_logLevel(self, aStr):
    # DBG.write("filter_logLevel(self, string) %s" % (aStr), True)
    value = LOG.filterLevel(aStr)
    # DBG.write("filter_logLevel(self, string) %s -> %s" % (aStr, value), True)
    return value

  def filter_list(self, string):
    """
    parser filter from string 'xx,yy,zz,...'
    returns list (if not error with python exec(value=[xx,yy,zz,...]))
    """
    try:
      if string[0] == "[":
        newstring = string[1:-1] # remove leading trailing []
      else:
        newstring = string
      newstring = "'" + newstring.replace(",", "','") + "'"
      # DBG.write("newstring" , newstring, True)
      if newstring != "''":
        aDict = {}
        exec("value = [%s]" % newstring, aDict)
        value = aDict['value']
      else:
        value = []
    except Exception as e:
      # msg = "%r is not a list" % string
      raise Exception(e)
    return value

  def filter_list_float(self, string):
    """
    parser filter from string 'xx,yy,zz,...'
    returns list (if not error with python exec(value=[xx,yy,zz,...]))
    """
    if True: #try:
      # exec("value = [%s]" % string)
      # DBG.write("filter_list_float" , string, True)
      value = self.filter_list(string)
      value = [float(v) for v in value]
    else: #except Exception as e:
      # msg = "%r is not a list of float" % string
      raise Exception(e)
    return value

  def filter_range(self, string):
    """
    parser filter from string 'vmin,vmax'
    returns list (if not error with python exec(value=[xx,yy]))
    """
    try:
      exec("value = [%s]" % string)
      value = [float(v) for v in value]
    except Exception as e:
      msg = "%r is not a list of float" % string
      raise Exception(e)
    if len(value) != 2:
      # msg = "%r is not a range of 2 float" % string
      raise Exception(e)
    return value

  def filter_list_int(self, string):
    """
    parser filter from string 'xx,yy,zz,...'
    returns list (if not error with python exec(value=[xx,yy,zz,...]))
    """
    try:
      exec("value = [%s]" % string)
      value = [int(v) for v in value]
    except Exception as e:
      # msg = "%r is not a list of int" % string
      raise Exception(msg)
    return value

  def filter_square(self, string):
    value = int(string)
    sqrtv = value ** (.5)
    if sqrtv != int(sqrtv):
      msg = "%r is not a perfect square" % string
      raise Exception(msg)
    return value

  def filter_int_positive(self, string):
    try:
      value = int(string)
    except:
      msg = "%r is not a correct positive integer number" % string
      raise Exception(msg)
    if value < 0:
      msg = "%r is not a positive integer number" % string
      raise Exception(msg)
    return value

  def filter_float_positive(self, string):
    try:
      value = float(string)
    except:
      msg = "%r is not a correct positive float number" % string
      raise Exception(msg)
    if value < 0:
      msg = "%r is not a positive float number" % string
      raise Exception(msg)
    return value

  def filter_float_positive_3d(self, string):
    try:
      value = self.filter_list_float(string)
    except:
      msg = "%r is not a correct positive float number" % string
      raise Exception(msg)
    if len(value) != 3:
      msg = "%r is not a correct list of 3 float number" % string
      raise Exception(msg)
    if False in [(v>=0) for v in value]:
      msg = "%r is not a correct list of 3 positive number" % string
      raise Exception(msg)
    return value

  def filter_float_3d(self, string):
    try:
      value = self.filter_list_float(string)
    except:
      msg = "%r is not a correct positive float number" % string
      raise Exception(msg)
    if len(value) != 3:
      msg = "%r is not a correct list of 3 float number" % string
      raise Exception(msg)
    return value

  def filter_existing_file(self, string):
    aRealFile = os.path.realpath(string)
    try:
      ok = os.path.isfile(aRealFile)
    except:
      msg = "incorrect existing file name %s" % aRealFile
      raise Exception(msg)
    if not ok:
      msg = "inexisting file %s" % aRealFile
      logger.warning(msg)  # continue with message
      # raise Exception(msg)  # stop with message
    return aRealFile

  def filter_file_pyconf(self, string):
    if ".pyconf" in string:
      value = string
    else:
      value = string + ".pyconf"
    """try:
      ok = os.path.isfile(string)
    except:
      msg = "%r is not a correct existing file name" % string
      raise Exception(msg)
    if not ok:
      msg = "%r is not an existing file" % string
      raise Exception(msg)
    """
    return value

  def getCmdNames(self):
    directory = os.path.dirname(__file__)
    res = glob.glob(os.path.join(directory, "cmd_*.py"))
    res = sorted([getNameCmdFromFileName(f) for f in res])
    return res

  def getCmdNamesStr(self):
    return "[" + "|".join(self.getCmdNames())+ "]"

  def filter_cmd_name(self, string):
    value = string
    if string == "NONE":
      return value
    if string not in self.getCmdNames():
      msg = "<critical>Unknown command name %r" % string
      logger.critical(msg + ", correct values are " + self.getCmdNamesStr())
      return "UNKNOWN"
    return value

  def filter_workdir(self, string):
    try:
      wdir = os.path.realpath(os.path.expandvars(string))
      DBG.write("arg workdir %s" % string, wdir, verbose)
      ok = os.path.isdir(string)
      value = wdir
    except Exception as e:
      msg = "%r is not a correct existing directory\n%s" % (string, e)
      raise Exception(msg)

    py4amitex_root_dir = os.getenv("PY4AMITEX_ROOT_DIR")
    DBG.write("py4amitex_root_dir", py4amitex_root_dir, verbose)
    if py4amitex_root_dir in wdir:
      msg = "working directory: %r\nhave to be outside directory: %r" % (wdir, py4amitex_root_dir)
      raise Exception(msg)
    # not existing directory: have to be created by user, or automatic.
    if not ok:
      msg = "%r is not an existing directory" % wdir
      raise Exception(msg)
    return value

  def getNamespaceStr(self, namespace):
    """could avoid import parserP4A, direct parser instance method"""
    return getNamespaceStr(namespace)


def getNamespaceStr(namespace):
  """
  return pretty print Namespace class
  namespace is returned instance of parse_args method
  """
  return PP.pformat(namespace.__dict__)

def getNameCmdFromFileName(aFileName):
  """from '.../cmd_tea.py' returns 'tea' """
  res = os.path.splitext(os.path.basename(aFileName))[0]
  res = res.split("cmd_")[1]
  return res

def get_common_parser():
  """
  Define all COMMON commands <options> for py4amitex CLI: 'LaunchP4A <options>'
  """
  workdirdefault = UP4A.getWorkdirDefault()
  # logger.info("parser PY4AMITEX_WORKDIR is %s" % workdirdefault)
  # ... xml ... or json ... or else
  fileinputdefault = os.path.join(workdirdefault, "P4A_in.xml")
  fileoutputdefault = os.path.join(workdirdefault, "P4A_out.xml")

  parser = ArgumentParserNoExit(
    description='command line interface for py4amitex stuff',
    argument_default=None, add_help=False)

  parser.add_argument(
    '-c', '--cmd',
    help='set command to proceed: %s default=%s' % (parser.getCmdNamesStr(), "NONE"),
    type=parser.filter_cmd_name,
    default="NONE",
    metavar='cmdName'
  )
  parser.add_argument(
    '-v', '--verbose',
    help='set log level verbosity: %s default=%s' % (parser.getLogLevelsStr(), "INFO"),
    type=parser.filter_logLevel,
    default="INFO",
    metavar='logLevel'
  )
  parser.add_argument(
    '-w', '--workdir',
    help="current working directory default=%s" % workdirdefault,
    type=parser.filter_workdir,
    default=workdirdefault,
    metavar='dirName'
  )
  """
  parser.add_argument(
    '-fi', '--fileinput',
    help="current file input default=%s" % fileinputdefault,
    type=parser.filter_existing_file,
    default=fileinputdefault,
    metavar='file.xxx'
  )
  parser.add_argument(
    '-fo', '--fileoutput',
    help="current file output default=%s" % fileoutputdefault,
    type=parser.filter_existing_file,
    default=fileoutputdefault,
    metavar='file.xxx'
  )
  """

  """
  # other examples
  parser.add_argument(
    '-p', '--pattern',
    help="file pattern for unittest files ['test_*.py'|'*Test.py'...]",
    default="test_???_*.py",  # as alphabetical ordered test site
    metavar='filePattern'
  )
  parser.add_argument(
    '-t', '--type',
    help="type of output: [std(standard ascii)|xml|html|hdf5]",
    default="std",
    choices=['std', 'xml', 'html', 'hdf5'],
    metavar='outputType'
  )"""

  parser.add_argument(
    '-h', '--help',
    help='show this help message (and do no exit)',
    default=False,
    action='store_true'
  )
  parser.add_argument(
    '-d', '--doc',
    help='show py4amitex documentation',
    default=False,
    action='store_true'
  )

  # DBG.write('dir(parser)', dir(parser), True)
  return parser
