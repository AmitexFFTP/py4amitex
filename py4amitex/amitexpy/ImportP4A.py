#!/usr/bin/env python
# -*- coding:utf-8 -*-


"""\
This file is the first import file to use py4amitex API,
in python jupyter or else, Linux only for now.

| Python usage:
| python
| >> import py4amitex.amitexpy.ImportP4A as IP4A
| >> mos = IP4A.get_instanceP4A()
| >> help(mos)
| >> mos.compute(someArguments)
"""

import os
import sys
import platform
import pprint as PP

# exit OKSYS and KOSYS seems equal on linux or windows
OKSYS = 0  # OK
KOSYS = 1  # KO

verbose = False

py4amitexdir = os.path.realpath(os.path.dirname(os.path.dirname(__file__)))
py4amitexworkdir = os.path.realpath(os.path.dirname(py4amitexdir))

# Make the package accessible from all code
if py4amitexdir not in sys.path:
  sys.path.insert(0, py4amitexdir)
  if verbose: print("sys.path.insert", py4amitexdir)

import py4amitex.loggingSimple as LOG

logger = LOG.getDefaultLogger()
logger.debug("python version %s" % platform.python_version())
logger.debug("current operating system %s" % platform.system())

# platform problems
if platform.system() == "Windows":
  logger.error("Windows NOT supported, abort.")
  sys.exit(KOSYS)  # stop there is problem
  """
  # may be tomorrow... coloring ansi, supershell, or not
  os.environ["LANG"] = "fr_FR.UTF-8"  # windows not present
  os.environ["USER_WEBBROWSER"] = "chrome"
  os.environ["USER_EDITOR"] = "notepad" # r"c:\Windows\System32\notepad.exe"
  homeDir = os.environ["USERPROFILE"]
  os.environ["HOME"] = homeDir # HOME (as linux) not present under windows
  """
else:
  os.environ["USER_WEBBROWSER"] = "firefox"
  os.environ["USER_EDITOR"] = "pluma"
  homeDir = os.environ["HOME"]
  os.environ["USERPROFILE"] = homeDir  # USERPROFILE (as windows) not present under linux


def get_instanceP4A():
  """returns a new Mos instance"""
  os.environ["PY4AMITEX_ROOT_DIR"] = py4amitexdir
  os.environ["PY4AMITEX_WORKDIR"] = py4amitexworkdir
  logger.debug("py4amitex file is %s" % os.path.realpath(__file__))
  logger.debug("PY4AMITEX_ROOT_DIR is %s" % py4amitexdir)

  try:
    # it is time to try complex stuff
    from py4amitex.amitexpy.ApiP4A import RunnerP4A
    instanceP4A= RunnerP4A()  # instantiate the RunnerP4A class
    return instanceP4A

  except Exception as e:
    # error as may be unknown problem
    # verbose debug message with traceback if developers
    msg = "Exception raised for execute mat"
    import py4amitex.debug as DBG  # Easy print stderr (for DEBUG only)
    logger.critical(DBG.format_color_exception(msg))
    # logger.info("END of %s" % os.path.basename(__file__))
    logger.close()  # important to close logger files (if present)
    return None




