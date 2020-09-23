#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Simple logger using logging package

| Define one logger with one handler on stdout ,
| and *not yet* one handler on file for production.
|
| Define another one logger for unittest with one stream handler,
| without console etc. outputs, but logs in memory for unittest,
| which have to be small.
|
| see: http://sametmax.com/ecrire-des-logs-en-python
|
| Define two Logger instances, no more need.
|   - _loggerDefault as production/development logger
|   - _loggerUnittest as unittest logger
|
| see use of handlers of _loggerDefault for
| log console and log files xml, txt
|
| console or file handlers:
|   - levels are 'critical, error, warning, info, debug'
|     they are formatted and indented on multi lines messages.
|
"""
import os
import sys
import logging
import pprint as PP
import platform
from types import MethodType

import py4amitex.loggerpy.coloring as COLS

verbose = False


############################################################################
def _indent(aStr, skip="\n           "):
  if "\n" in aStr:
    s = aStr.split("\n")
    return skip.join(s)
  else:
    return aStr


def _indentColoring(aStr, skip="\n           "):
  s = _indent(aStr + '<reset>', skip)  # trailing reset unconditionnaly
  s = COLS.toColor(s)
  return s


def formatColoring(self, record):
  """post-treatment format color formatter, is tricky, assumed"""
  # print("formatColoring record", dir(record))
  s = self._format(record)
  # coloring '[DEBUG   ]' etc., or not if redirect stdout in file
  if COLS.is_stdout_tty():
    s = COLS.replace(s, _tagsColoring)
  return s


# keep it simple too much colors kills color
_tagsColoring = [
  ("[DEBUG   ]", "[" + COLS.BLUE + "DEBUG   " + COLS.RESET + "]"),
  ("[INFO    ]", "[" + COLS.GREEN + "INFO    " + COLS.RESET + "]"),
  ("[WARNING ]", "[" + COLS.RED + "WARNING " + COLS.RESET + "]"),
  ("[ERROR   ]", "[" + COLS.YELLOW + "ERROR   " + COLS.RESET + "]"),
  ("[CRITICAL]", "[" + COLS.YELLOW + "CRITICAL" + COLS.RESET + "]"),
]

_knownLevels = "CRITICAL ERROR WARNING INFO DEBUG".split()
_knownLevelsStr = "[%s]" % "|".join(_knownLevels)


#################################################################
# utilities methods
#################################################################

def filterLevel(aLevel):
  """
  filter levels logging values from firsts characters levels.
  No case sensitive.

  | example:
  | 'i' -> 'INFO'
  | 'cRiT' -> 'CRITICAL'
  """
  aLev = aLevel.upper()
  knownLevels = _knownLevels
  maxLen = max([len(i) for i in knownLevels])
  for i in range(maxLen):
    for lev in knownLevels:
      if aLev == lev[:i]:
        # DBG.write("filterLevel", "%s -> %s" % (aLevel, lev))
        return lev
  msg = "Unknown level '%s', accepted are: %s" % (aLev, ", ".join(knownLevels))
  raise Exception(msg)
  # or signal problem but message debug
  # DBG.write("Problem filterLevel", msg, True)
  # return "CRITICAL"


def getStdoutHandler():
  """see logging.basicConfig method"""
  hdlr = logging.StreamHandler(sys.stdout)
  fs = "[%(levelname)-8s] %(message)s"  # format
  dfs = None  # datefmt
  fmt = logging.Formatter(fs, dfs)
  fmt._format = fmt.format
  # tricky but works for tag level coloring
  fmt.format = MethodType(formatColoring, fmt)
  hdlr.setFormatter(fmt)
  return hdlr


def getMemoryHandler():
  """see logging.basicConfig method"""
  from logging.handlers import MemoryHandler
  hdlr = MemoryHandler(capacity=10000)
  fs = "[%(levelname)-8s] %(message)s"  # format
  dfs = None  # datefmt
  fmt = logging.Formatter(fs, dfs)
  fmt._format = fmt.format
  # tricky but works for tag level coloring
  # fmt.format = MethodType(formatColoring, fmt)
  hdlr.setFormatter(fmt)
  return hdlr


############################################################################
class SimpleLogger(logging.Logger):
  """
  coloring, and indent multi lines in logging messages
  """

  def __repr__(self):
    return "SimpleLogger('%s')" % self.name

  def critical(self, msg, *args, **kwargs):
    if self.isEnabledFor(logging.CRITICAL):
      self._log(logging.CRITICAL, _indentColoring(msg), args, **kwargs)

  def error(self, msg, *args, **kwargs):
    if self.isEnabledFor(logging.ERROR):
      self._log(logging.ERROR, _indentColoring(msg), args, **kwargs)

  def warning(self, msg, *args, **kwargs):
    if self.isEnabledFor(logging.WARNING):
      self._log(logging.WARNING, _indentColoring(msg), args, **kwargs)

  def info(self, msg, *args, **kwargs):
    if self.isEnabledFor(logging.INFO):
      self._log(logging.INFO, _indentColoring(msg), args, **kwargs)

  def debug(self, msg, *args, **kwargs):
    if self.isEnabledFor(logging.DEBUG):
      self._log(logging.DEBUG, _indentColoring(msg), args, **kwargs)

  def test_log_color(self):
    test_log_color(self)

  def setHandlersLevel(self, level):
    """
    Why are there two setLevel() methods?
    The level set in the logger determines which severity of messages
    it will pass to its handlers.
    The level set in each handler determines which messages
    that handler will send on.
    """
    for h in self.handlers:
      h.setLevel(level)

  def close(self):
    if verbose:
      print("logger %s close" % self.name)
    for h in self.handlers:
      try:
        h.close()
      except:  # inexistent method close() as useless for stdout stderr
        pass


def setLoggerSimple(log, handler=None):
  log.setLevel("DEBUG")
  if handler is not None:
    log.addHandler(handler)
  return


# not set/known in logging manager, avoid use .
_loggerDefault = SimpleLogger("defaultSimple")
_loggerUnittest = SimpleLogger("unittestSimple")

setLoggerSimple(_loggerDefault, getStdoutHandler())
setLoggerSimple(_loggerUnittest, getMemoryHandler())

if verbose:
  print("create logger %s" % _loggerDefault)
  print("create logger %s" % _loggerUnittest)


def getLogger(name="default"):
  if name == "unittest":
    res = _loggerUnittest
  elif name == "default":
    res = _loggerDefault
  else:
    res = getLogger(name)
  if verbose: print("getLogger '%s'" % res.name)
  return res


def getDefaultLogger():
  return getLogger("default")


def getUnittestLogger():
  return getLogger("unittest")


def test_log(log):
  log.info("test logger %s" % log)
  log.debug("test_log a log debug")
  log.info("test_log a log info")
  log.warning("test_log a log warning")
  log.error("test_log a log error")
  log.critical("test_log a log critical\nmulti_line critical message\n")


def test_log_color(log):
  log.debug("test_log_color <debug>a log debug")
  log.info("test_log_color <info>a log info")
  log.warning("test_log_color <warning>a log warning")
  log.error("test_log_color <error>a log error")
  log.critical("test_log_color <critical>a log critical")


if __name__ == "__main__":
  """
  The level set in the logger determines which severity of messages
  it will pass to its handlers.
  """
  _loggerDefault.setLevel("DEBUG")
  _loggerUnittest.setLevel("DEBUG")
  test_log(_loggerDefault)
  test_log(_loggerUnittest)
  test_log_color(_loggerDefault)
