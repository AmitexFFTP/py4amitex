#!/usr/bin/env python

"""\
Console_script main entry for py4amitex package
in mode Command Line Argument(s) (CLI), Linux only

| Usage:
| >> .../LaunchP4A -h
|
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

py4amitexworkdir = os.getenv("PY4AMITEX_WORKDIR")
if py4amitexworkdir is None:
  py4amitexworkdir = "${HOME}/PY4AMITEX_WORKDIR"
py4amitexworkdir = os.path.realpath(os.path.expandvars(py4amitexworkdir))

# Make the package accessible from all code
if py4amitexdir not in sys.path:
  sys.path.insert(0, py4amitexdir)
  if verbose: print("sys.path.insert", py4amitexdir)

import py4amitex.loggerpy.loggingSimple as LOG
logger = LOG.getDefaultLogger()

def run():

  # prerequisites
  #################################

  if verbose:
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

  import py4amitex.debugpy.debug as DBG  # Easy print stderr (for DEBUG only)

  # main stuff
  #################################

  os.environ["PY4AMITEX_ROOT_DIR"] = py4amitexdir
  os.environ["PY4AMITEX_WORKDIR"] = py4amitexworkdir
  # logger.debug("py4amitex main file is %s" % os.path.realpath(__file__))

  try:
    # it is time to do complex stuff
    from py4amitex.amitexpy.ApiP4A import RunnerP4A, getLogo

    logger.info(getLogo())
    if verbose:
      logger.debug("environment var PY4AMITEX_ROOT_DIR is %s" % py4amitexdir)
      logger.debug("environment var PY4AMITEX_WORKDIR is %s" % py4amitexworkdir)
    instanceP4A = RunnerP4A()  # instantiate the RunnerP4A class
    rc = instanceP4A.execute_cli(sys.argv[1:])  # do complex stuff
    if not rc.isOk():
      logger.error(str(rc))
      logger.info("END of %s" % os.path.basename(__file__))
      logger.close()  # important to close logger files (if present)
      sys.exit(KOSYS)
    else:
      logger.info("END of %s" % os.path.basename(__file__))
      logger.close()  # important to close logger files (if present)
      sys.exit(OKSYS)

  except Exception as e:
    # error as may be unknown problem
    # verbose debug message with traceback if developers
    msg = "Exception raised for execute mat"
    logger.critical(DBG.format_color_exception(msg))
    logger.info("END of %s" % os.path.basename(__file__))
    logger.close()  # important to close logger files (if present)
    sys.exit(KOSYS)


def runSimple():
  """simple test for setup entry_point, or else"""
  logger.info("amitexpy.LaunchP4A.run hello __file__ " + __file__)
  sys.exit(OKSYS)



#################################
# __main__ and else
#################################


if __name__ == '__main__':  # direct launch
  if verbose: logger.debug("amitexpy.LaunchP4A __main__ begin")
  run()
  sys.exit(OKSYS)

elif __name__ == 'py4amitex.amitexpy.LaunchP4A': # direct launch from py4amitex pip entry_points
  if verbose: logger.debug("amitexpy.LaunchP4A package_entry_point begin")
  run()
  sys.exit(OKSYS)

else:
  logger.critical("forbidden or unexpected mode for __name__ '%s'" % __name__)
  logger.close()  # important to close logger files (if present)
  sys.exit(KOSYS)

