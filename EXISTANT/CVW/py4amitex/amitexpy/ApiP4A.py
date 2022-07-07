#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
This file is the main API file for py4amitex in CLI

| Warning: NO '__main__ ' call allowed,
|          Use 'LaunchP4A'
|
| Usage:
| import py4amitex.amitexpy.ApiP4A as AP4A
| see file LaunchP4A.py
"""

import os
import sys
import pprint as PP


# exit OKSYS and KOSYS seems equal on linux or windows
OKSYS = 0  # OK
KOSYS = 1  # KO

########################################################################
# NO __main__ entry allowed, use LaunchP4A
########################################################################
if __name__ == "__main__":
  msg = """
ERROR: 'ApiP4A.py' is not main command entry (CLI) for py4amitex.
     Use 'LaunchP4A' instead.\n\n"""
  sys.stderr.write(msg)
  sys.exit(_KOSYS)

import py4amitex
import py4amitex.amitexpy.utilsP4A as UP4A
import py4amitex.debugpy.debug as DBG  # Easy print stderr (for DEBUG only)
import py4amitex.loggerpy.loggingSimple as LOG
import py4amitex.utilspy.utils as UTS
from py4amitex.returncodepy.returnCode import ReturnCode
from py4amitex.amitexpy.parserP4A import get_common_parser, getNamespaceStr

logger = LOG.getDefaultLogger()


########################################################################
# some utilities methods
########################################################################

def getVersion():
  """get version number as string"""
  return py4amitex.__version__

def assumeAsList(strOrList):
  """return a list (like sys.argv) if strOrList is a string"""
  if type(strOrList) is list:
    return list(strOrList)  # copy
  else:
    res = strOrList.split(" ")
    return [r for r in res if r != ""]  # supposed string to split for convenience

def getLogo():
  """http://patorjk.com/software/taag/#p=display&w=%20&f=Big&t=py4amitex"""
  res = r'''
              _  _                  _ _            
             | || |                (_) |           
  _ __  _   _| || |_ __ _ _ __ ___  _| |_ _____  __
 | '_ \| | | |__   _/ _` | '_ ` _ \| | __/ _ \ \/ /
 | |_) | |_| |  | || (_| | | | | | | | ||  __/>  < 
 | .__/ \__, |  |_| \__,_|_| |_| |_|_|\__\___/_/\_\
 | |     __/ |                                     
 |_|    |___/                                      
'''
  return res


########################################################################
# RunnerP4A class make all stuff
########################################################################

# a one line bash pretty git log
_git_hist = """git --no-pager log -2 --pretty=format:"%h %ad | %s%d <%an>" --graph --date=short"""
# a simple bash git status
_git_stat = """git status --porcelain -b"""
# a formatted bash date
_date = """date '+%y_%m_%d_%H_%M'"""


########################################################################
class RunnerP4A(object):
  """
  The main class that computes all the commands of py4amitex from CLI
  """

  def __init__(self):
    # logger.debug("py4amitex RunnerP4A.__init__()")
    self.args = None
    self.options = None
    # in case question and user answer(s) in interactive keyboard/console
    self._confirmMode = True
    self._batchMode = False
    self.parser = get_common_parser()
    pass

  def version(self):
    return __version_info__

  def __repr__(self):
    aDict = {
      "arguments": self.args,
      "options": self.options,
    }
    tmp = PP.pformat(aDict)
    res = "RunnerP4A(\n %s\n)\n" % tmp[1:-1]
    return res

  def parseArguments(self, arguments, verbose=False):
    args = assumeAsList(arguments)
    if verbose: logger.info("args are " + PP.pformat(args))
    if self.parser is None:
      self.parser = get_common_parser()

    options, unknown_args = self.parser.parse_known_args(args) # returns tuple
    if len(unknown_args) == 0:
      return options

    """"
    # have to be first unknown
    if unknown_args[0] not in self.known_commands:
      logger.critical("unknown command argument %s" % unknown_args)
      raise Exception("unknown command argument %s" % unknown_args)
    """

    if verbose:
      logger.info("py4amitex options:\n\n<data>%s<reset>\n" % getNamespaceStr(options))
      logger.info("py4amitex unknown options %s" % PP.pformat(unknown_args))
    return options

  def get_help(self):
    """get general help colored string"""
    msg = self.getColoredVersion() + "\n\n"
    msg += self.parser.format_help()
    return msg

  def print_help(self):
    """prints py4amitex general help"""
    logger.info(self.get_help())
    for cmd in self.parser.getCmdNames():
      CMD = self.cmd_import(cmd)
      logger.info("<header>--cmd %s\n%s<reset>" % (cmd, CMD.__doc__))

  def show_doc(self):
    """show py4amitex general documentation as doc/index.html"""
    nameBrowser = os.getenv("USER_WEBBROWSER") # self.options.webbrowser
    root_dir = os.getenv("PY4AMITEX_ROOT_DIR")
    # fileIndex = os.path.join(root_dir, "README.md")
    fileIndex = os.path.join(root_dir, "doc/index.html")
    cmd = "%s --new-window %s" % (nameBrowser, fileIndex)
    logger.info("active show doc command is\n%s" % cmd)
    UTS.Popen(cmd, logger=logger, wait="no")

  def getColoredVersion(self):
    """get colored version message"""
    version = getVersion()
    msg = "<header>py4amitex version:<reset> " + version
    return msg

  def getConfirmMode(self):
    """used in case question and user answer(s) in interactive keyboard/console"""
    return self._confirmMode

  def setConfirmMode(self, value):
    """used in case question and user answer(s) in interactive keyboard/console"""
    self._confirmMode = value

  def getBatchMode(self):
    return self._batchMode

  def getAnswer(self, msg):
    """
    in case question and user answer(s) in interactive console
    if confirm mode and not batch mode.

    | returns 'YES' or 'NO' if confirm mode and not batch mode
    | returns 'YES' if batch mode
    """
    logger = self.getLogger()
    if self.getConfirmMode() and not self.getBatchMode():
      logger.info("\n" + msg)
      rep = eval(input(_("Do you want to continue? [yes/no] ")))
      if rep.upper() == _("YES"):
        return "YES"
      else:
        return "NO"
    else:
      logger.debug(msg)  # silent message
      logger.debug("<green>YES<reset> (as automatic answer)")
      return "YES"

  def execute_cli(self, cli_arguments=[]):
    """parse cli_arguments and make stuff.
    From here we lazy import needed modules and use classes

    :param cli_arguments: (str or list) The py4amitex CLI arguments (as sys.argv)
    """
    self.args = assumeAsList(cli_arguments)
    args = self.args  # shorcut
    # no arguments do print help and returns
    if len(args) == 0:
      self.print_help()
      return ReturnCode("OK", "No arguments, as '--help'")

    # parse common options
    self.options = self.parseArguments(args)
    options = self.options  # shortcut

    # set main handler level
    logger.setHandlersLevel(options.verbose)
    # if options.verbose == 'DEBUG' : logger.test_log_color()
    # logger.info(PP.pformat(dir(logger.handlers[0])))

    if options.cmd == 'UNKNOWN':
      return ReturnCode("KO", "exit as --cmd UNKNOWN ")

    if options.doc:
      self.show_doc()
      return ReturnCode("OK", "exit if arguments contains '--doc'")

    if not os.path.isdir(options.workdir):
      msg = "inexisting working directory %s\nyou have to create it." % options.workdir
      return ReturnCode("KO", msg)

    logger.debug("Mat.execute_cli() on args %s" % self.args)
    logger.debug("common arguments options:\n\n<data>%s<reset>\n" % getNamespaceStr(options))

    # if the help option has been called, print command help
    if options.help:
      if options.cmd == "NONE":
        self.print_help()
        # and not continue if no --cmd
        return ReturnCode("OK", "exit as arguments contains only '--help'")

    if self.args is None:
      logger.error("Mat.execute_cli() needs args not empty")
      self.execute_cli("-h")
      return

    # check tests stuff
    #####################

    workdir = UP4A.getWorkdirDefault()
    logger.info("current workdir is %s" % workdir)

    # TODO other PY4AMITEX checks, presence of import uranie python (for example...)

    # checks done, options stuff
    #####################################################

    """ 
    # obsolete example...
    if xxx not in sys.path:
      # Make other package accessible from all code
      sys.path.insert(0, xxx)
      logger.info("sys.path.insert %s" % xxx)
    """

    # needs other import etc
    #####################################################

    if options.cmd == 'UNKNOWN':
      return ReturnCode("KO", "--cmd UNKNOWN")

    if options.cmd == 'NONE':
      logger.warning("inexisting --cmd argument as nothing to do")
      return ReturnCode("OK", "--cmd NONE as nothing to do")

    #res = self.compute_test() # for debug
    res = self.compute()
    return res


  def compute_test(self):
    """(here for debug) import cmd_tea and compute on args"""
    import py4amitex.amitexpy.cmd_tea as CMD
    res = CMD.compute(self)
    return res

  def cmd_import(self, cmd_xxx):
    """import cmd_xxx as CMD and returns CMD"""
    aDict = {}
    importStr = "import py4amitex.amitexpy.cmd_%s as CMD" % cmd_xxx
    try:
      exec(importStr, aDict)
      CMD = aDict['CMD']
    except Exception as e:
      logger.critical("problem in %r" % importStr)
      raise Exception(e)
    return CMD


  def compute(self):
    """import cmd_xxx as CMD and call CMD.compute()"""
    CMD = self.cmd_import(self.options.cmd)
    # here comes command stuff
    res = CMD.compute(self)
    return res
