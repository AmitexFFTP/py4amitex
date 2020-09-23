#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
simple tagging as '<color>' for simple coloring log messages on terminal(s)
unix ansi colors, may be window or ios compatible 
Using '<color>' because EZ human readable,
So '<color>' are not supposed existing in log message.
"{}".format() is not choosen because "{}" are present
in log messages of contents of python dict (as JSON) etc.
for more colors, backgound, bold etc
use colorama package https://github.com/tartley/colorama

| Usage:
| >> import iradinapy.coloringIra as COLS
| 
| Example:
| >> log("this is in <green>color green<reset>, OK is in blue: <blue>OK?")
"""

import os
import sys
import platform
import pprint as PP

_verbose = True
_name = "coloring"

verbose = False

"""
ansi color stuff but only foreground colors:
Available formatting constants are:
Foreground: BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, RESET.
Style: DIM, NORMAL, RESET_ALL

note: DIM is not assumed in win32
"""

CSI = '\033['

BLACK           = CSI + str(30) + 'm'
RED             = CSI + str(31) + 'm'
GREEN           = CSI + str(32) + 'm'
YELLOW          = CSI + str(33) + 'm'
BLUE            = CSI + str(34) + 'm'
MAGENTA         = CSI + str(35) + 'm'
CYAN            = CSI + str(36) + 'm'
WHITE           = CSI + str(37) + 'm'
RESET           = CSI + str(39) + 'm'

# order matters for items replaces forward to color
_tags = [
  ("<black>", BLACK),
  ("<red>", RED),
  ("<green>", GREEN),
  ("<yellow>", YELLOW),
  ("<blue>", BLUE),
  ("<magenta>", MAGENTA),
  ("<cyan>", CYAN),
  ("<white>", WHITE),
  ("<reset>", RESET),
  ("<header>", BLUE),
  ("<label>", CYAN),
  ("<data>", MAGENTA),
  ("<success>", GREEN),
  ("<debug>", BLUE),
  ("<info>", GREEN),
  ("<warning>", RED),
  ("<error>", YELLOW),
  ("<critical>", YELLOW),
]

# _tagsNone = ( (i, "") for i,j in _tags ) # to clean tags when log not tty
# reversed order matters for item replaces backward to no color
_tagsNone = list( reversed( [(i, "") for i, j in _tags] ) )

# more non empty colored smart tags
_tags = _tags + [
  ("<OK>", GREEN + "OK" + RESET),
  ("<KO>", RED + "KO" + RESET),
]

# more non empty colored smart tags reversed order
_tagsNone = [
  ("<OK>", "OK"),
  ("<KO>", "KO"),
] + _tagsNone

# import pprint as PP
# print("_tags %s" % PP.pformat(_tags))
# print("_tagsNone %s" % PP.pformat(_tagsNone))

def indent(msg, nb, car=" "):
  """indent nb car (spaces) multi lines message except first one"""
  s = msg.split("\n")
  res = ("\n"+car*nb).join(s)
  return res

def log(msg):
  """elementary log stdout for debug if _verbose"""
  prefix = "%s: " % _name
  nb = len(prefix)
  if _verbose: 
    ini = prefix + indent(msg, nb)
    res = toColor(ini)
    if res != ini: # there was <xxx>, append reset
      res = res + RESET # + toColor("<reset>")
    print(res)
  
class ColoringStream(object):
  """
  write my stream class
  only write and flush are used for the streaming
  https://docs.python.org/2/library/logging.handlers.html
  https://stackoverflow.com/questions/31999627/storing-logger-messages-in-a-string
  """
  def __init__(self):
    self.logs = ''

  def write(self, astr):
    # log("UnittestStream.write('%s')" % astr)
    self.logs += astr
    # TODO test if too big

  def flush(self):
    pass

  def __str__(self):
    return self.logs

def is_stdout_tty():
  try:
    res = sys.stdout.isatty()
  except:
    res = False
  if verbose:
    sys.stderr.write("is_stdout_tty %s\n" % res)
  return res  

def toColor(msg):
  """
  automatically clean the message of color tags '<red> ... 
  if the terminal output stdout is redirected by user
  if not, replace tags with ansi color codes
  """
  istty = is_stdout_tty()
  if not istty:
    # clean the message color (if the terminal is redirected by user)
    return replace(msg, _tagsNone)
  elif platform.system() == "Windows":
    # clean the message color (is there color in Anaconda prompt or cmd.exe ???)
    return replace(msg, _tagsNone)
  else:
    # https://en.wikipedia.org/wiki/ANSI_escape_code#Escape_sequences
    # rc + CSI 0 K as clear from cursor to the end of the line
    # s = msg.replace("<RC>", "\r\033[0;K")
    return replace(msg, _tags)
    
def cleanColors(msg):
  """clean the message of color tags '<red> ... """
  return replace(msg, _tagsNone)

def replace(msg, tags):
  s = msg
  for r in tags:
    s = s.replace(*r)
  return s
  
if __name__ == "__main__":  
  log("<green>green coloring in <blue>%s" % __file__)
  log("<yellow>yellow coloring<reset> in <blue>%s" % __file__)
  log("<magenta>magenta coloring<reset> in <blue>%s" % __file__)
  log("<warning>warning coloring (as red) in <blue>%s" % __file__)
  log("<cyan>cyan coloring and not reset colors...")
  log("...and here is not cyan because appended reset at end of message")
  log("colored <OK> or <KO> implicitly")
  # log("dir():\n<blue>%s" % PP.pformat(dir()))


  
