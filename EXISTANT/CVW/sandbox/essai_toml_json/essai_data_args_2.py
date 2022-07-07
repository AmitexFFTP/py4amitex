#!/usr/bin/env python
# -*- coding: utf-8 -*-

r"""
just tries for simple data input light as dict/list python code
with future compatible argparse
This is ONE of many solutions (JSON etc as export for c/fortan compiled codes)
this is permits use on \n in strings, why not ?
this is permits use of # comment
this is permit json dumps to get data normalization, if needed.
seems OK, **one of many solutions**

warning in dict python AND json duplicate keys selects last one *without warning*

https://www.programiz.com/python-programming/methods/built-in/compile
https://opensource.com/article/18/4/introduction-python-bytecode
https://stackoverflow.com/questions/16064409/how-to-create-a-code-object-in-python
"""

import json
import pprint as PP

##########################################
def myprint(tit, obj):
  print("\n********** ppformat %s %s **********\n\n%s" % (tit, type(obj), PP.pformat(obj)))

##########################################
class RangeEncoder(json.JSONEncoder):
  def default(self, obj):
    # print(type(obj))
    if isinstance(obj, range):
      return [i for i in obj]
    # Let the base class default method raise the TypeError
    return json.JSONEncoder.default(self, obj)

def jsonprint(tit, obj):
  print("\n********** jsondumps %s %s **********\n\n%s" %
        (tit, type(obj),
         json.dumps(obj, cls=RangeEncoder, indent=2)))


##########################################
# theses methods could be somewhere else general singleton module
def check_int(value):
  if type(value) is type(1): return True
  return False

def check_float(value):
  if type(value) is type(1.): return True
  return False

def check_list(value):
  if type(value) is type([]): return True
  return False

def check_list_int(value):
  if type(value) is type([]):
    for i in value:
      if not check_int(i): return False
    return True
  return False

def check_list_float(value):
  if type(value) is type([]):
    for i in value:
      print("check_list_float item", i, check_float(i))
      if not check_float(i): return False
    return True
  return False

##########################################
class DataInput(object):

  def __init__(self, data=None, pattern=None):
    self.pattern = pattern
    self.data = None
    if data is not None:
      self.load(data)

  def load(self, data_exec):
    """
    exec code object compiled python string code with result as DATA_IN
    """
    aDict = {}
    try:
      codeObject = compile(data_exec, 'unknown_file', 'exec')
      # myprint("codeObject", dir(codeObject))
      exec(codeObject, aDict)
    except Exception as e:
      print("\nERROR:\n%s" % data_exec) # may be line number
      raise Exception(e)
    try:
      self.data = aDict['DATA_IN']
    except:
      raise Exception("unknown 'DATA_IN' variable in result namespace of:\n%s" % data_exec)
    return self.data

  def check(self, checktyp):
    """
    not recursive, only for example
    could be other clever useful solution
    """
    for key, value in self.data.items():
      typ = checktyp.data[key]
      aDict = {
        "value": value,
        "check_int": check_int,
        "check_float": check_float,
        "check_list": check_list,
        "check_list_int": check_list_int,
        "check_list_float": check_list_float,
      }
      aTest = "ok = %s(value)" % typ
      # print("check %r" % aTest)
      exec(aTest, aDict)
      ok = aDict["ok"]
      print("check %r" % key, value, typ, ok)
      if not ok: return False
    return True

  def __str__(self):
    return "DataInput(\n%s\n)\n" % PP.pformat(self.data)

##########################################
def essai_0():
  codeInString = '''
a = 5
b = 6
sum = a + b # with comment...
# print("sum =", sum)
'''
  codeObject = compile(codeInString, 'sumstring', 'exec')
  aDict = {}
  exec(codeObject, aDict)
  del aDict['__builtins__']
  myprint('essai_0', aDict)

##########################################
def essai_1():
  myData = DataInput()
  myData.load('DATA_IN = ["foo", {"bar":["baz", None, 1.0, 2]}]')
  myprint('essai_1', myData.data)
  
##########################################
def essai_2():
  myData = DataInput()
  myData.load('''
import math
temp_value = math.pi * 2.
# in dict duplicate keys select last one without warning
DATA_IN = [
  "foo", 
  {
    "bar":
    [
      r"baz",  # comment accepted...
      None,
      """hello
 ... one more line from hello ...
""",
      temp_value,
      temp_value * 2.,  # expressions accepted...
      1.0, 
      2 + 55,  # operators accepted...
    ],
    "range1": [float(i) for i in range(1, 10)], # range evaluated !
    "range2": range(1, 5),  # range accepted! , and iterator keeped BUT not JSON serializable
  }
]
''')
  myprint('essai_2', myData.data)
  jsonprint('essai_2', myData.data)

def essai_3():
  """example of generation iterations"""
  myData = DataInput()
  myData.load('''
import math
temp_value = math.pi * 2.
# in dict duplicate keys select last one without warning
DATA_IN = {
  "par_1": temp_value,
  "nb_myArray": None,
  "myArray": None
}
''')

  # this is without control types or values...
  jsonprint('essai_3 initial', myData.data)
  # overrides changes data on the fly
  for i in range(1,5):
    myData.data["nb_myArray"] = i
    myData.data["myArray"] = [float(ii) for ii in range(0, i)]
    jsonprint('essai_3 i=%i' % i, myData.data)


def essai_4():
  """
  example
  of generation iterations with example of checking types
  this is ONE of clever solutions
  """
  myData = DataInput()
  myData.load('''
import math
temp_value = math.pi * 2.
# in dict duplicate keys select last one without warning
DATA_IN = {
  "par_1": temp_value,
  "nb_myArray": None,
  "myArray": None
}
''')

  # define types somewhere else ...
  # "check_float" etc as string avoid future
  # cross python imports error
  myType = DataInput()
  myType.load('''
DATA_IN = {
  "par_1": "check_float",
  "nb_myArray": "check_int",
  "myArray": "check_list_float"
}
''')

  # this is without control types or values...
  jsonprint('essai_3 initial data', myData.data)
  jsonprint('essai_3 initial type', myType.data)

  # overrides changes data on the fly
  for i in range(1,5):
    myData.data["nb_myArray"] = i
    myData.data["myArray"] = [float(ii) for ii in range(0, i)]
    jsonprint('essai_3 i=%i' % i, myData.data)
    ok = myData.check(myType)
    print("check type seems to be %s" % ok)


if __name__ == "__main__":
  # myprint("dir(DataInput())", dir(DataInput()))
  if False:
    essai_0()
    essai_1()
    essai_2()
    essai_3()
  essai_4()

