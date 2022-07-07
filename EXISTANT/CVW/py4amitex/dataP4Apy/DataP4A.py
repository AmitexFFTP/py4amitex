#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
This file is the main file for py4amitex DataP4A class
This class is central class to assume API for

- data **inputs/outputs** assumed as a tree
- **data manipulations** in amitex_fft data world,
  generation of parametric study, (DOE? not yet)
- simple type and range checking, optionally
- simple default data value, optionally

This is try to be a "easy to use" class for a python programmer.
KISS principle as ONE class for 80% functionality needs
(Using some python trick which are NOT so simple)

User **have to study** elementaries uses
in test_xxx_DataP4a.py which are The Only F. Reference For Use


| Usage:
| import py4amitex.dataP4Apy.DataP4A as DP4A
"""

import os
import platform
import numpy as np
import pandas as pd
import pprint as PP
import argparse as AP
import json
import h5py


# json.validate() as DP4A.json_validate()
# as LAZY raise error if import jsonschema fail
# in case of absence jsonschema user could use DataP4A nevertheless
# without jsonschema functionalities
try:
  from jsonschema import validate as json_validate
except:
  def json_validate(*arg, **kw):
    raise Exception("Impossible to import jsonschema, fix it.")



import py4amitex
import py4amitex.debugpy.debug as DBG  # Easy print stderr (for DEBUG only)
import py4amitex.loggerpy.loggingSimple as LOG
import py4amitex.utilspy.utils as UTS
from py4amitex.returncodepy.returnCode import ReturnCode

logger = LOG.getDefaultLogger()
verbose = False  # True # in debug
verbose1 = False # print all insert item in dict
verbose2 = False # print something in debug



########################################################################
# some utilities methods
########################################################################


_HDF5_ARRAYS = {} # KISS only global singleton (for now), only init in dumpStrJsonHdf5, which is NOT recursive

# needed leading 'x' to create hdf5 tags compatible
# tables/path.py:155: it does not match the pattern ``^[a-zA-Z_][a-zA-Z0-9_]*$``
_HDF5_FMT = 'x%03i_%s_%s'  # needed 'x'
_HDF5_TAGS = {np.ndarray: "ndarray", pd.core.frame.DataFrame: "dataframe"}


def _HDF5_write_tag(typeData, path):
  nb = len(_HDF5_ARRAYS)
  typ = _HDF5_TAGS[typeData]
  res = _HDF5_FMT % (nb, typ, path)
  return res

def jsonprint(tit, obj):
  print("\n********** jsondumps %s %s **********\n\n%s" %
        (tit, type(obj),
         json.dumps(obj, cls=DataP4AEncoder, indent=2)))

def jsonlog(tit, obj):
  logger.info("********** jsondumps %s %s **********\n\n%s" %
        (tit, type(obj),
         json.dumps(obj, cls=DataP4AEncoder, indent=2)))

def lineIndent(aStr):
  """Indent and numerote lines"""
  res = "\n"
  for i, line in enumerate(aStr.split('\n')):
    res += '\n%000i  %s' % (i+1, line)
  return res + '\n'

########################################################################
# DataP4AEncoder JSON classes
########################################################################

class DataP4AEncoder(json.JSONEncoder):
  """
  JSON Encoder for complete JSON dump, useless for serialisation of ndarray and dataframe
  """
  _className = "DataP4AEncoder"

  def default(self, obj):
    # print("********* DataP4AEncoder.default", type(obj))
    if isinstance(obj, range):
      return [i for i in obj]
    if obj.__class__ == DataP4ALeaf:
      res = obj._value
      # res could be all python types of object (from user or not)
      # assume it as a JSON tree LEAF
      if res.__class__ in [bool, int, float, None.__class__, str]:
        # assume here for JSON known types what to do
        return res

      # assume here for JSON unknown types what to do
      # here we can do something specific for np.array, for example
      # TODO something as write in file or pickle asci dump
      # TODO set name of file as 'file:\\...' for big data, or create zip multiple files
      if res.__class__ in _HDF5_TAGS.keys():
        # logger.debug("Unexpected leaf data type %s in %s, truncated representation." % (type(res), self._className))
        return _HDF5_TAGS[res.__class__] + str(res.shape)
      #  res = str(res.dumps()) # as bytes to str
      #  print("TODO may be something else for numpy big arrays", res, str(res), res.decode('utf-16')
      #  return res


      logger.error("Unexpected leaf data type %s in %s" % (type(res), self._className))
      res = res.__repr__()
      return res

    # Let the base class default method raise the TypeError
    return json.JSONEncoder.default(self, obj)


class DataP4AEncoderResume(json.JSONEncoder):
  """
  JSON Encoder for partial resume (human eyes only) JSON dump, useless for serialisation of ndarray and datafram
  """
  _className = "DataP4AEncoderResume"

  def default(self, obj):
    # print("********* DataP4AEncoderResume.default", type(obj))
    if isinstance(obj, range):
      return [i for i in obj]
    if obj.__class__ == DataP4ALeaf:
      res = obj._value
      # res could be all python types of object (from user or not)
      # assume it as a JSON tree LEAF
      if res.__class__ in [bool, int, float, None.__class__, str]:
        return res
      if res.__class__ in _HDF5_TAGS.keys():
        return "%s(%s)" % (_HDF5_TAGS[res.__class__], str(res))

      # assume here for DataP4A JSON unknown types what to do
      logger.info("Leaf data type ignored %s in %s" % (type(res), self._className))
      return str(type(res))


    # Let the base class default method raise the TypeError
    return json.JSONEncoder.default(self, obj)



class DataP4AEncoderHdf5(json. JSONEncoder):
  """
  JSON Encoder extended to numpy.ndarray and pandas.dataframe for
  - partial result is tree asci resume (human eyes only) JSON dump
  - and complementary result extended global singleton DP4A._HDF5_ARRAYS dict
    to a posteriori get included leaves numpy.ndarray, pandas.core.frame.DataFrame,
    with index_named python tree path for a posteriori dump HDF5 files
  see class DataP4A method 'def dumpFileHdf5'
  """
  _className = "DataP4AEncoderHdf5"

  def default(self, obj):
    # print("********* DataP4AEncoderResume.default", type(obj))
    if isinstance(obj, range):
      return [i for i in obj]

    # res could be all python types of object (from user or not)
    # assume it as a JSON tree LEAF
    if obj.__class__ == DataP4ALeaf:
      res = obj._value

      if res.__class__ in _HDF5_TAGS.keys():
        aPath = obj.getPythonPath()
        tag = _HDF5_write_tag(res.__class__, aPath)

        # logger.debug("Leaf data type %s %s in %s" % (type(res), aPath, self._className))

        if aPath in _HDF5_ARRAYS:
          mess = 'Problem %s is in _HDF5_ARRAYS yet' % aPath
          logger.error(mess)
          return mess

        _HDF5_ARRAYS[tag] = res # warning
        return tag

      elif res.__class__ not in [bool, int, float, None.__class__, str]:
        # assume here for JSON unknown types what to do
        # here we can do something specific for np.array, for example
        logger.error("Unexpected leaf data type %s in %s" % (type(res), self._className))
        return str(type(res))

      else:
        return res

    # Let the base class default method raise the TypeError
    return json.JSONEncoder.default(self, obj)


########################################################################
# _DataP4ABase class JSON dict
########################################################################

class _DataP4ABase(object):
  """Common methods for DataP4Axxx classes"""

  def _castValue(self, value):
    """try to cast value to DataAP4xxx instances"""
    clsname = self.__class__.__name__
    # print("%s._castValue" % (clsname, value))

    # DataP4A objet setattr as tree, assume _parent attribute
    # try to cast value to DataAP4xxx
    if True:
      # logger.info("%s._castValue %s" % (clsname, value.__class__))
      if value.__class__ == dict:
        castValue = DataP4ADict(value) # casting or deep copy from dict
        return castValue

      if value.__class__ == list:
        castValue = DataP4AList(value) # casting or deep copy from list
        return castValue

      if value.__class__ in [DataP4A, DataP4ADict, DataP4AList]: # theorically ok
        return value

      # supposed immutables as leaves of tree
      # NO cast None and bool as not derivables class, sorry
      # cast int float str with for _parent attribute and python path.

      if value.__class__ in [bool, None.__class__]:
        # not derivables class, sorry, tant pis faire avec...
        # suppose only problem on python path as no _parent, 
        #secondary functionality accessoire.
        # castValue = DataP4ALeaf(value) # do not that as implementation choice
        return value

      if value.__class__ == int:
        castValue = DataP4AInt(value)
        return castValue

      if value.__class__ == float:
        castValue = DataP4AFloat(value)
        return castValue

      if value.__class__ == str:
        castValue = DataP4AStr(value)
        return castValue

      if value.__class__ in [np.ndarray, pd.core.frame.DataFrame]:
        castValue = DataP4ALeaf(value)
        return castValue

      # as value.__class__ not in [dict, list, bool, int, float, None.__class__, str] json compatible
      # as user modifying data, possibly he knows what he is doing, 
      # only warning
      logger.warning('''casting user value, hope you know what you are doing with %s
in expected json-compatible DataP4A tree''' % type(value)) #, PP.pformat(value)))
      # continue and see what will happend
      castValue = DataP4ALeaf(value)
      return castValue

  def getPythonPath(self, root=''):
    """
    return something like '.heroes.Haddock' from node job ad'hoc (...capitain!)
    could be useful method for __ future__ functionality findInTree(aNodeName)
    (NOT IMPLEMENTED YET 08/2020)
    """
    # raise Exception("TODO DataP4ADict.getPythonPath not improved")
    res = ''
    current = self
    for i in range(1,10): # avoid infinite loops deep 10 iterate algorithm (not recursive), no more
      if current._parent is None: # not present yet on immutables leaves...
        return root + res # TODO 'root'->'' only for newbie understanding
      else:
        path = current._parent._getPythonChildPath(current)
        res = path + res
        # print("+++ path deep %i %r %s" % (i, res, current))
      current = current._parent
    raise Exception("Maximum parent loop reached %i" % i)

  def validate(self, json_schema, verbose=False):
    dict_valid = json.loads(json_schema)
    dict_to_test = self # DataP4A seen as dict
    ok = True
    try:
      json_validate(dict_to_test, dict_valid)
    except Exception as e:
      logger.warning("""validation KO in
      
<blue>%s

<red>%s
""" % (self.dumpStrJson(), e))
      ok = False
    return ok

  def _hdf5_shape2str(self, aShape, verbose=False):
    # help(aShape)
    if verbose: DBG.write("_hdf5_shape2str aShape", aShape, True)
    if verbose: DBG.write("_hdf5_shape2str dir(aShape)", dir(aShape), True)

    aArray = np.array(aShape)
    if verbose: DBG.write("_hdf5_shape2str aArray", aArray, True)
    if verbose: DBG.write("_hdf5_shape2str dir(aArray)", dir(aArray), True)

    aStr = aArray.tobytes().decode('UTF-8')  # ouffff
    if verbose: print("_hdf5_shape2str type %s\n%s" % (aStr))
    return aStr

  def _setPyPath(self, aPyPath, aValue, verbose=False):

    if verbose: logger.info("_setPyPath self%s <--- %s" % (aPyPath, aValue))
    aDict = {"anObject": self, "aValue": aValue}
    cmd = "anObject%s = aValue" % aPyPath  # simple meta prog, KISS on errors also...
    try:
      exec(cmd, aDict)
    except Exception as e:
      msg = DBG.format_color_exception(str(e))
      logger.error("Executing '%s'\n%s" % (cmd, msg))
      # choose to continue...


  def json_validate(self, jsonschema):
    return json_validate(self, jsonschema)

  '''
  # is not KISS
  def _setPyPath_obsolete(self, aPyPath, aValue):

    # always relative path, so no need first "."
    if aPyPath[0] == '.':
      paths = aPyPath[1:].split(".")
    else:
      paths = aPyPath.split(".")

    # logger.info("_setPyPath %s ---> %s" % (aPyPath, paths))
    s = self
    if len(paths) > 1:
      for attr in paths[0:-1]:
        if '[' in attr:
          raise Exception("Problem in getattr '%s'" % attr)
        s = getattr(s, attr)

    # setattr(s, paths[-1], aValue) creates "another_list[0]": "dataframe(3, 3)"
    attr = paths[-1]
    if '[' in attr:
      raise Exception("Problem in setattr '%s'" % attr)
    setattr(s, attr, aValue)'''


  def _getPyPath(self, aPyPath):
    paths = aPyPath.split(".")
    if len(paths) == 0 :
      return None
    s = self
    for attr in paths:
      s = getattr(s, attr)
    return s


  def _get_HDF5_arrays_paths(self, res):
    """no recursive arrays_path as 'x123_dataframe_.pypath' only valid in a DataP4AStr"""
    if self.__class__ is DataP4AStr:
      # print("\n_get_HDF5_arrays_paths", self, res)
      tmp = self.split("_dataframe_.")
      if len(tmp) == 2:
        res[tmp[0] + "_dataframe_"] = '.' + tmp[1]

      tmp = self.split("_ndarray_.")
      if len(tmp) == 2:
        res[tmp[0] + "_ndarray_"] = '.' + tmp[1]
    return

  def get_HDF5_arrays_paths(self, res, verbose=False):
    """assume DataP4A tree recursion"""

    # print("\nxxxxxxx get_HDF5_arrays_paths", self.__class__)
    current = self
    if current.__class__ is DataP4AStr:
      current._get_HDF5_arrays_paths(res)
      return

    if current.__class__ in [DataP4ADict, DataP4A]:
      # print("DataP4ADict")
      for nam, value in current.items():
        # print("get_HDF5_arrays_paths DataP4ADict value", nam)
        try:
          value.get_HDF5_arrays_paths(res)
        except:
          pass
      return

    if current.__class__ is DataP4AList:
      # print("get_HDF5_arrays_paths DataP4AList value")
      for value in current:
        try:
          value.get_HDF5_arrays_paths(res)
        except:
          pass
      return




########################################################################
# DataP4ADict class JSON dict
########################################################################

class DataP4ADict(dict, _DataP4ABase):
  """
  The main class that store py4amitex data as a dict of DataP4A instances,
  Considerate this dict could be a TREE, if contains other DataP4ADict or DataP4AList
  This class IS DESIGNED to be ROOT of one data tree
  allowing python syntax code for manipulation of data
  use named method load/dump as json do
  """

  _className = "DataP4ADict"

  def __init__(self, *arg, **kw):

    # print('\nDataP4ADict.__init__', arg)

    if arg == ([],):
      # print("args as empty list")
      raise Exception('DataP4ADict init by EMPTY LIST forbidden')

    self._parent = None

    if len(arg) >= 1:
      # print("len(arg) >= 1", arg)
      if dict == arg[0].__class__:
        super().__init__(arg[0], **kw) # from __dict__ as easy fix
      elif AP.Namespace == arg[0].__class__:
        # accept init as cast from result parser AP.Namespace.__dict__
        super().__init__(arg[0].__dict__, **kw) # from __dict__ as easy fix
      else:
        raise Exception("DataP4ADict problem init arg %s" % arg)
    else:
      # other standard init()
      # print("standard dict init()")
      super().__init__(*arg, **kw)

    # here because constructor dict from arg do not use __setitem__
    # implement casting of values
    for key, v in self.items():
      self[key] = v # casting by __setitem__


  def __setattr__(self, attr, value):
    if verbose: logger.debug("DataP4ADict.__setattr__ %r = %s" % (attr, value))
    if attr[0] == '_': # value as classical setattr
      return super().__setattr__(attr, value)
    castValue = self._castValue(value)
    if hasattr(castValue, "_parent"):
      if castValue._parent is not None:
        raise Exception("DataP4ADict.__setattr__ attr %r value mandatory have _parent None\n%s" % (key, value))
    try:
      castValue._parent = self
    except Exception as e:
      # DBG.write("Exception", e, verbose)
      DBG.write("TODO have to test have to be immutable value %r" % value, "", verbose)
      pass
    return super().__setitem__(attr, castValue) # set attribute not '_xxx' as dict key yeah


  def __setitem__(self, key, value):
    if not issubclass(key.__class__, str):
      raise Exception("DataP4ADict.__setitem__ key %r mandatory have to be str" % key)
    DBG.write("DataP4ADict.__setitem__ %r <- %r" % (key, value), "", verbose)
    castValue = self._castValue(value)
    if hasattr(castValue, "_parent"):
      if castValue._parent is not None:
        raise Exception("DataP4ADict.__setitem__ key %r value mandatory have _parent None\n%s" % (key, value))
    try:
      castValue._parent = self
    except Exception as e:
      # DBG.write("Exception", e, verbose)
      DBG.write("TODO have to test have to be immutable value %r" % value, "", verbose)
      pass
    return super().__setitem__(key, castValue)

  def __repr__(self):
    return "DataP4ADict(" + super().__repr__() + ")"

  def __repr__pretty__(self):
    """
    PrettyPrinter works well if you don't care about the order of the keys
    workaround python 3.8 only:
      PP.PrettyPrinter(sort_dicts=False).pprint(a._data)
    here keys are alphabetically sorted
    this method is NOT designed for (ascii) serialization
    use toJson or toPy or else toXml for that
    """
    tmp = PP.pformat(self)
    res = "DataP4ADict(\n%s\n)\n" % tmp[1:-1]
    return res

  def __repr_with_parent__(self):
    """to debug parent tree functionality"""
    return "DataP4ADict(\n_parent=%s,\nself=%s\n)" % (self._parent, super().__repr__()) + ")"

  def _getPythonChildPath(self, child):
    # print("%%%% DataP4ADict._getPythonChildPath child", child)
    for key, v in self.items():
      if id(v) == id(child):
        return '.' + key
    raise Exception("DataP4ADict child\n%s\n-- not in --\n%s\n" % (child, self))

  def _getkeys(self):
    """
    return simple list from self.keys() instead dict_keys class
    Avoid 'dict_keys()' write format, example : "dict_keys(['_parent'])
    """
    return [k for k in self.keys()]

  def __getattr__(self, attr, verbose=False):
    """
    if not begins '_' this attribute is considerate as item of dict
    which is considerate as a branch or leaf of a tree
    allows 'instanceDict.keyname' shorter coding way, alternative of instanceDict["keyname"]
    """
    DBG.write("DataP4ADict.__getattr__ %r" % attr, "")
    # DBG.write("dir(DataP4ADict)", dir(self), True)
    if "_" != attr[0]:
      try:
        return self[attr]
      except Exception as e:
        logger.error("Problem inexisting key %r, existing are '%s'" % (attr, self._getkeys()))
        raise Exception(e)
    else:
      # '_xxx' as 'classical' attribute of dict class (as self._parent is)
      DBG.write("DataP4ADict.__dict__ classical attribute %r in" % attr, self.__dict__, verbose)
      try:
        return self.__dict__[attr]
      except Exception as e:
        raise Exception("DataP4ADict attribute %r not found in %s" % (attr, self._getkeys()))

  def _getitem_value(self, attr):
    """
    assume if item is DataP4ALeaf instance return DataP4ALeaf._value
    do that and abandon DataP4A methods and _parent information...
    """
    DBG.write("DataP4ADict._getitem_value %r" % attr, "")
    res = super().__getitem__(attr)
    if res.__class__ == DataP4ALeaf:
      res = res._value
    return res

  def getDate(self):
    from datetime import datetime
    res = datetime.now().strftime("%d/%m/%Y %H:%M:%S")
    return res

  def loadStrPy(self, data_exec, verbose=False):
    """
    exec code object as compiled python string code
    data input as result as 'DATA_IN' dictionary.

    loadStrPy use python code syntax for input data, no limits!
    avoid if then else etc. please, it is DATA, NOT program.
    """
    aDict = {}
    try:
      codeObject = compile(data_exec, 'input_stream', 'exec')
      # myprint("codeObject", dir(codeObject))
      exec(codeObject, aDict)
    except Exception as e:
      msg = "DataP4ADict problem in input_stream python:\n%s\n" % (lineIndent(data_exec))
      msg = DBG.format_color_exception(msg)
      if verbose: logger.critical(msg)
      raise Exception(e) # stoop
    if 'DATA_IN' in aDict:
      DBG.write("DataP4ADict.loadStrPy", aDict['DATA_IN'], verbose)
      self._initFromDict(aDict['DATA_IN'])
    else:
      raise Exception("DataP4ADict unknown 'DATA_IN' variable in result namespace of:\n%s" % data_exec)
    return

  def _initFromDict(self, aDict):
    """clear self  and copy aDict items in self, recusive"""
    self.clear()
    for key, v in aDict.items():
      castedval = v
      if v.__class__ == dict: # recusive for dict
        DBG.write("_initFromDict %r key value is dict, need to cast as DataP4ADict" % (key), v, verbose1)
        castedval = DataP4ADict()
        castedval._initFromDict(v)

      if v.__class__ == list: # recusive for list
        DBG.write("_initFromDict %r key value is list, need to cast as DataP4AList" % (key), v, verbose1)
        castedval = DataP4AList()
        castedval._initFromList(v)

      self[key] = castedval  # theoricaly __setitem__ set _parent
      if castedval.__class__ in [DataP4ADict, DataP4AList]:
        DBG.write("ok %s parent of %s key %r" % (id(self), id(v), key), "", verbose)
        # if castedval._parent is not None:
        if castedval._parent is not self: # may be done in previous self[key] = castedval
            DBG.write("_initFromDict__ key %r (%s) have not _parent as expected" % (key, castedval.__class__), castedval._parent.__repr__(), True)
            raise Exception("_initFromDict__ key %r (%s) have not _parent as expected" % (key, castedval.__class__))
        # castedval._parent = self
        # DBG.write("v.__repr_with_parent__()", v.__repr_with_parent__(), verbose1 )


  def loadFileJson(self, name_data_json, verbose=True):
    with open(name_data_json, "r") as f:
      return self.loadStrJson(f.read())

  def loadFilePy(self, name_data_py, verbose=True):
    with open(name_data_py, "r") as f:
      return self.loadStrPy(f.read())


  def loadStrJson(self, data_json, verbose=True):
    """
    load data input as Json text, result as dictionary.

    loadStrJson use json package and normalized syntax for input data, no comments!
    it is mandatory DATA, NO program tricks allowed.
    """
    try:
      aDict = json.loads(data_json)
    except Exception as e:
      msg = "DataP4ADict problem in input_stream json:\n%s\n" % (lineIndent(data_json))
      msg = DBG.format_color_exception(msg)
      if verbose: logger.critical(msg)
      raise Exception(e) # stoop

    self._initFromDict(aDict)
    return

  def dumpStrJson(self):
    """simple classical JSON dumps, do not accept numpy array"""
    # DBG.write("dumpStrJson", self, True)
    res = json.dumps(self, cls=DataP4AEncoder, indent=2)
    return res

  def dumpStrJsonResume(self):
    """classical JSON dumps accept and resume numpy array etc, with warning"""
    # DBG.write("dumpStrJsonResume", self, True)
    res = json.dumps(self, cls=DataP4AEncoderResume, indent=2)
    return res

  def _dumpStrJsonHdf5(self):
    """utility to HDF5 with JSON dumps, accept numpy array etc, only internal use"""
    # DBG.write("_dumpStrJsonHdf5", self, True)
    global _HDF5_ARRAYS
    _HDF5_ARRAYS = {} # global singleton to store tree numpy arrays
    res = json.dumps(self, cls=DataP4AEncoderHdf5, indent=2)
    return res

  def dumpFileHdf5(self, nameFile, display=False, compression=None):
    """
    HDF5 dump in file with JSON dumps for simple tree leaves, accept numpy array pandas dataframe
    creates file as loadFileHdf5 load file

    dumpFileHdf5/loadFileHdf5 uses
    - json strings in hdf5 datasets to store DataP4A tree simple data (boolean, int, float, simple string)
    - np arrays    in hdf5 datasets to store numpy (big) array data and pandas dataframe

    json strings are multi-line strings, leads to store items in HDF5 with dataset np.string_
    see https://docs.h5py.org/en/2.10.0/strings.html:
    'This is the most-compatible way to store a string. Everything else can read it'

    optional display result file with vitables

    Lossless compression filters
      GZIP filter ("gzip"
        Available with every installation of HDF5,
        so it’s best where portability is required.
        Good compression, moderate speed.
        compression_opts sets the compression level and may be an integer from 0 to 9, default is 4.
      LZF filter ("lzf")
        Available with every installation of h5py (C source code also available).
        Low to moderate compression, very fast. No options.

    WARNING:
      dataframes compression using complib : {'zlib', 'bzip2', 'lzo', 'blosc', None}, default None
      !!! what ??? gzip is not a valid compression option (and is ignored, that's a bug).
      try any of zlib, bzip2, lzo, blosc (bzip2/lzo might need extra libraries installed)
      => avoid dataframes compression dataframes for now
    """

    if compression in [None, 0, "no"]:
      compr = None
    elif compression in [1, "yes"]:
      compr = "gzip"
    elif compression in ["gzip", "lzf"]:
      compr = compression
    else:
      logger.warning("hdf5 compression unknown '%s'" % compression)
      compr = None
    # DBG.write("dumpFileHdf5", self, True)
    global _HDF5_ARRAYS
    isDataFrames = False
    with h5py.File(nameFile, 'w') as f:

      # info
      ginf = f.create_group('meta_informations')
      aInfo = DataP4A()
      aInfo.version = "1.0.0"
      aInfo.file_name_origin = os.path.basename(nameFile)
      aInfo.hostname_origin = platform.node()
      aInfo.username_origin = UTS.getUser()

      # useless aInfo.file_directory_name = os.path.dirname(nameFile)
      aInfo.file_date = self.getDate()
      aStr = aInfo._dumpStrJsonHdf5() # here prefer not existing arrays
      ginf.create_dataset(name="info_hdf5_file", data=np.string_(aStr))

      # tree simple data as JSON human readable
      gdat = f.create_group('data')
      aStr = self._dumpStrJsonHdf5() # fill _HDF5_ARRAYS
      gdat.create_dataset(name="data_json", data=np.string_(aStr))

      # tree numpy arrays as dataset compressed or not
      garr = f.create_group('arrays')
      for p, val in _HDF5_ARRAYS.items():

        typ = p.split(".")[0]
        # garr = f.create_group('arrays/' + p)  # p.split(".")[0]

        if "ndarray" in typ:
          # print("typ %s ---> %s " % (typ, p))
          if compr is None:
            garr.create_dataset(name=typ, data=val)
          else:
            garr.create_dataset(name=typ, data=val, compression=compr)
        elif "dataframe" in typ:
          isDataFrames = True
          pass # append later with to_hdf
        else:
          logger.error("dumpFileHdf5 problem with %s" % p)

    # append dataframe(s) now
    if isDataFrames:
      store = pd.HDFStore(nameFile, "a")
      for p, val in _HDF5_ARRAYS.items():
        typ = p.split(".")[0]
        if "dataframe" in typ:
          # logger.info("dumpFileHdf5 %s ---> %s " % (typ, p))
          # val.to_hdf(nameFile, key="arrays/" + p, mode='a')
          store.put("arrays/" + typ, val)
      store.close()


    _HDF5_ARRAYS = {} # raz useless dict
    if display: os.system("vitables " + nameFile)
    return None

    if verbose:
      logger.info("get_HDF5_arrays_paths\n%s" % PP.pformat(res))
    return

  '''def loadFileHdf5_obsolete(self, nameFile, verbose=False):
    """
    load HDF5 file as dumpFileHdf5 creates file

    dumpFileHdf5/loadFileHdf5 uses a mix json string in hdf5 dataset to store DataP4A tree data
    """
    try:
      with h5py.File(nameFile, 'r') as f:  # read hdf5 file
        if verbose:
          ng = '/meta_informations/info_hdf5_file'
          aShape = f[ng]
          aStr = self._hdf5_shape2str(aShape)
          logger.info("meta_informations/info_hdf5_file:\n%s" % aStr)

        aShape = f['/data/data_json']
        aStr = self._hdf5_shape2str(aShape)
        if verbose: logger.info("data/data_json:\n%s" % aStr)

        self.loadStrJson(aStr) # load without arrays

        HDF5_arrays = {}
        self.get_HDF5_arrays_paths(HDF5_arrays)

        # load arrays
        ng = 'arrays'
        msg = 'groupe %s:\n' % ng
        group = f['/arrays']
        j = 0
        for nsg, aShape in group.items(): # or in f[ng].items():
          j += 1
          aPyPath = ".".join(nsg.split(".")[1:]) # from "001.xxxx.yyy" to "xxxx.yyy"

          aArray = np.array(aShape)
          self._setPyPath(aPyPath, aArray)
          msg += "array%s '%s' pypath '%s'\n" % (str(aArray.shape), aShape.name, aPyPath)

        if verbose: logger.info(msg)

    except Exception as e:
      msg = "DataP4ADict problem in load HDF5 file %s: %s" % (nameFile, e)
      msg = DBG.format_color_exception(msg)
      if verbose: logger.error(msg)
      return None

    return None'''

  def loadFileHdf5(self, nameFile, verbose=True):
    """
    load HDF5 file as dumpFileHdf5 creates file

    dumpFileHdf5/loadFileHdf5 uses a mix json string in hdf5 dataset to store DataP4A tree data
    """
    try:
      with h5py.File(nameFile, 'r') as f:  # read hdf5 file
        if verbose:
          ng = '/meta_informations/info_hdf5_file'
          aShape = f[ng]
          aStr = self._hdf5_shape2str(aShape)
          logger.info("meta_informations/info_hdf5_file:\n%s" % aStr)

        aShape = f['/data/data_json']
        aStr = self._hdf5_shape2str(aShape)
        if verbose: logger.info("data/data_json:\n%s" % aStr)

        self.loadStrJson(aStr)  # load without arrays

        HDF5_arrays = {}
        self.get_HDF5_arrays_paths(HDF5_arrays, verbose=True)
        if verbose: logger.info("arrays_paths:\n%s" % PP.pformat(HDF5_arrays))

        # load np.ndarrays
        ng = '/arrays'
        msg = 'groupe %s:\n' % ng
        group = f[ng]

        dataFrames = []
        for nsg, aShape in group.items():

          if "_dataframe_" in nsg:
            dataFrames.append(nsg)  # read it later with pd.HDFStore
            continue

          # nsg is "_123_ndarray_" or "_456_dataframe_" etc
          if nsg not in HDF5_arrays.keys():
            logger.error("loadFileHdf5 reference '%s' not found" % nsg)
            continue
          aPyPath = HDF5_arrays[nsg] # to ".xxxx.yyy"

          aArray = np.array(aShape)
          if verbose: logger.info("loadFileHdf5 %s ---> %s\n%s" % (nsg, aPyPath, aArray))
          self._setPyPath(aPyPath, aArray)
          msg += "array%s '%s'   pypath '%s'\n" % (str(aArray.shape), aShape.name, aPyPath)


      # f for read std is with closed
      # now f is read pd.HDFStore
      if len(dataFrames) > 0:
        with pd.HDFStore(nameFile, "r") as f:
          for nsg in dataFrames:
            if nsg not in HDF5_arrays.keys():
              logger.error("loadFileHdf5 reference '%s' not found" % nsg)
              continue
            aPyPath = HDF5_arrays[nsg] # to ".xxxx.yyy"
            name = '/arrays/' + nsg
            df = f.get(name)
            if verbose: logger.info("loadFileHdf5 %s ---> %s\n%s" % (nsg, aPyPath, df))
            self._setPyPath(aPyPath, df)
            msg += "array%s '%s' pypath '%s'\n" % (str(df.shape), name, aPyPath)

      # f for read pd.HDFStore is with closed
      if verbose: logger.info(msg)


    except Exception as e:
      msg = "DataP4ADict problem in load HDF5 file %s: %s" % (nameFile, e)
      msg = DBG.format_color_exception(msg)
      if verbose: logger.error(msg)
      return None

    return None

  def dumpStrPy(self):
    strJson = self.dumpStrJson()
    strPy = json.loads(strJson)
    # return str(strPy) # one line initial ordered
    return PP.pformat(strPy) # indented line but sorted



########################################################################
# DataP4AList class JSON list
########################################################################

class DataP4AList(list, _DataP4ABase):
  """
  The secondary class that store py4amitex data as a list of DataP4A instances
  Considerate this list could contains some dicts as DataP4ADict,
  This class IS NOT DESIGNED to be ROOT of data trees
  allowing python syntax code for manipulation of data
  NO use (inexisting) named method load/dump as json could do, because is secondary class.
  """

  _className = "DataP4AList"

  def __init__(self, *arg, **kw):
    # print('DataP4AList.__init__', arg)
    if arg == ({},):
      # print("args as empty dict")
      raise Exception('DataP4AList init by EMPTY DICT forbidden')

    ko = False
    try: # if arg != ():
      if arg[0].__class__ == dict:
        ko = True
    except:
      pass

    if ko: raise Exception('DataP4AList init by DICT forbidden')

    # if arg == ([],):
    #   print("args as empty list")
    # if arg == ():
    #   print("no args")

    self._parent = None
    super().__init__(*arg, **kw)   # inherited class list constructor
    # here because list constructor from arg do not use __setitem__
    for i, v in enumerate(self):
      self[i] = v # casting by __setitem__

  def _setparent(self, v):
    ko = False
    try:
      if v._parent is not None:
        ko = True
      else:
        v._parent = self
    except:
      pass

    if ko: raise Exception("DataP4AList._setparent %s mandatory have _parent None, yet value is :\n" % (self, v))

  def __setitem__(self, i, v):
    if verbose2: print("%%% DataP4AList.__setitem__")
    castValue = self._castValue(v)
    self._setparent(castValue)
    return super().__setitem__(i, castValue)

  def insert(self, i, v):
    if verbose2: print("%%% DataP4AList.insert")
    castValue = self._castValue(v)
    self._setparent(castValue)
    return super().insert(i, castValue)

  def append(self, v):
    if verbose2: print("%%% DataP4AList.append")
    castValue = self._castValue(v)
    self._setparent(castValue)
    return super().append(castValue)

  def extend(self, v):
    if verbose2: print("%%% DataP4AList.extend")
    castValue = self._castValue(v)
    for i in castValue:
      self._setparent(i)
    return super().extend(castValue)

  def insert(self, i, v):
    if verbose2: print("%%% DataP4AList.insert")
    castValue = self._castValue(v)
    self._setparent(castValue)
    return super().insert(i, v)

  def _getPythonChildPath(self, child):
    # print("%%%% DataP4AList._getPythonChildPath child", child)
    for i, v in enumerate(self):
      if id(v) == id(child):
        return '[%i]' % i
    raise Exception("DataP4AList child\n%s\n-- not in --\n%s\n" % (child, self))


  def _initFromList(self, aList):
    """clear self and copy aList items in self, recusive"""
    del self[:] # his actually removes the contents from the list, but doesn't replace with a new empty list
    for i, v in enumerate(aList):
      castedval = v
      if v.__class__ == dict: # recusive for dict
        DBG.write("_initFromList index %i value is dict, need to cast as DataP4ADict" % i, v, verbose1)
        castedval = DataP4ADict()
        castedval._initFromDict(v)

      if v.__class__ == list: # recusive for list
        DBG.write("_initFromList index %i value is list, need to cast as DataP4AList" % i, v, verbose1)
        castedval = DataP4AList()
        castedval._initFromList(v)

      self.append(castedval)  # theoricaly __append__ set _parent
      if castedval.__class__ in [DataP4ADict, DataP4AList]:
        DBG.write("ok %s parent of %s index %i" % (id(self), id(v), i), "", verbose)

        """if castedval._parent is not None:
          raise Exception("DataP4ADict.__initFromDict__ key %r (%s) mandatory have _parent None" % (key, castedval.__class__))
        castedval._parent = self
        #DBG.write("v.__repr_with_parent__()", v.__repr_with_parent__(), verbose1 )"""

        if castedval._parent is not self: # may be done in previous self.append(castedval)
            DBG.write("_initFromList index %i (%s) have not _parent as expected" % (i, castedval.__class__), castedval._parent.__repr__(), True)
            raise Exception("_initFromDict index %i (%s) have not _parent as expected" % (i, castedval.__class__))
        # castedval._parent = self
        # DBG.write("v.__repr_with_parent__()", v.__repr_with_parent__(), verbose1 )


########################################################################
# DataP4A class JSON leaf of tree
# as json classes [int, float, str] in dataP4A structure
# These classes are immutable
########################################################################

"""
[bool, int, float, None.__class__, str] as all immutables
- as immutable so you can't modify it after they are created
- Since __init__ is called after the object is constructed,
  it is too late to modify the value for immutable types.
- see http://stackoverflow.com/questions/2673651/inheritance-from-str-or-int
- see https://docs.python.org/2/reference/datamodel.html
"""

#################################
class DataP4AInt(int, _DataP4ABase):
  """
  DataP4AInt as all integers json compatible
  """

  _defaultValue = 0

  def __new__(cls, value=None):
    if value == None:
      val = cls._defaultValue
    else:
      val = value
      if value.__class__ != int: # do no accept other casting
        raise Exception("DataP4AInt: '%s' mandatory as integer"  % value)
    cls._parent = None
    return int.__new__(cls, val)

  def __repr__(self):
    #__repr__ goal is to be unambiguous
    return self.__class__.__name__ + '(' + int.__repr__(self) + ')'

#################################
class DataP4AFloat(float, _DataP4ABase):
  """
  DataP4AFloat as all float json compatible
  """

  _defaultValue = 0.

  def __new__(cls, value=None):
    if value == None:
      val = cls._defaultValue
    else:
      val = value
      if value.__class__ != float: # do no accept other casting
        raise Exception("DataP4AFloat: '%s' mandatory as float"  % value)
    cls._parent = None
    return float.__new__(cls, val)

  def __repr__(self):
    #__repr__ goal is to be unambiguous
    return self.__class__.__name__ + '(' + float.__repr__(self) + ')'


#################################
class DataP4AStr(str, _DataP4ABase):
  """
  DataP4AStr as all str json compatible
  n.b. python 3 str is unicode
  """
  _defaultValue = ""

  def __new__(cls, value = None):
    if value == None:
      val = cls._defaultValue
    else:
      val = value
      if value.__class__ != str: # do no accept other casting
        raise Exception("DataP4AFloat: '%s' mandatory as str"  % value)
      # val = str(value) # encoding ? not yet
    cls._parent = None
    return str.__new__(cls, val)

  def __repr__(self):
    #__repr__ goal is to be unambiguous
    return self.__class__.__name__ + '(' + str.__repr__(self) + ')'



########################################################################
# DataP4ALeaf class JSON leaf of tree
# as user useful classes (big numpy.array etc...) in dataP4A structure
# These classes are considered as almost-immutable
# KISS use but not abuse.
########################################################################

class DataP4ALeaf(_DataP4ABase):
  """
  as leaf of DataP4a tree as almost-immutable
  contains user-type instances which are not json compatible
  (example is big numpy.array  ...)
  user have to assume output serialisation in DataP4AEncoder
  as almost-immutable so you can't modify self._value after they are created

  'immutable' help:
  - as immutable so you can't modify it after they are created
  - Since __init__ is called after the object is constructed,
    it is too late to modify the value for immutable types.
  - see http://stackoverflow.com/questions/2673651/inheritance-from-str-or-int
  - see https://docs.python.org/2/reference/datamodel.html
  """

  _className = "DataP4ALeaf"
  # _forbiddenTypes as bug because managed in other previous classes
  _forbiddenTypes = [dict, list, bool, int, float, None.__class__, str] 

  # yes, "NOT DEFINED", because None could be is a valid known possible JSON data
  _defaultValue = "# NOT DEFINED #"

  def __init__(self, value):
    self._parent = None
    if value.__class__ in self._forbiddenTypes:
      raise Exception('''Type value %s forbidden in DataP4ALeaf,)
  fix it is using DataP4A/Dict/List/Int/Float/Str instead.''' % type(value))
    # here we could forbid some types of class, but why?
    super().__setattr__('_value', value)

  def __setattr__(self, attr, value):
    """
    almost immutable so OK to set attribute value as classical
    only if attribute named '_xxx', excluding '_value' for almost-immutability
    so you can't modify it after they are created
    """
    if verbose: logger.debug("DataP4ALeaf.__setattr__ %r = %s" % (attr, value))
    if attr[0] == '_' and attr != '_value':
      # set attribute value as classical only if set attribute '_xxx' as '_parent' (which is useful!)
      return super().__setattr__(attr, value)
    raise Exception("DataP4ALeaf set attribute %r forbiden as almost immutable, for value\n%s" % (attr, value))

  def __repr__(self):
    #__repr__ goal is to be unambiguous
    return '%s(%s)' % (self._className, self._value.__repr__())

  def __str__(self):
    return str(self._value)

  def __eq__(self, val):
    """trick for test unittest assertEqual for example"""
    res = (self._value == val)
    # DBG.write("DataP4ALeaf.__eq__ %r == %r -> %s\n" % (self._value, val, res), "", True)
    return res

  def __ne__(self, val):
    """trick for test unittest assertEqual for example"""
    res = (self._value != val)
    return res

  '''
  TODO other arithmetic __add__ etc ???
  example from dir(float) ??? Noooooo !!! ... NOT KISS
   
     '__abs__', '__add__', '__bool__', '__class__', '__delattr__',
     '__dir__', '__divmod__', '__doc__', '__eq__', '__float__',
     '__floordiv__', '__format__', '__ge__', '__getattribute__',
     '__getformat__', '__getnewargs__', '__gt__', '__hash__',
     '__init__', '__init_subclass__', '__int__',
     '__le__', '__lt__', '__mod__', '__mul__', '__ne__', '__neg__',
     '__new__', '__pos__', '__pow__', '__radd__', '__rdivmod__',
     '__reduce__', '__reduce_ex__', '__repr__', '__rfloordiv__',
     '__rmod__', '__rmul__', '__round__', '__rpow__', '__rsub__',
     '__rtruediv__', '__set_format__', '__setattr__', '__sizeof__',
     '__str__', '__sub__', '__subclasshook__', '__truediv__',
     '__trunc__', 'as_integer_ratio', 'conjugate', 'fromhex',
     'hex', 'imag', 'is_integer', 'real'
     
  '''


########################################################################
# DataP4A class
########################################################################

class DataP4A(DataP4ADict):
  """
  As newbie user standard TREE ROOT data, use DataP4A.
  TREE ROOT should not be a DataP4AList instance,
  but is expected aDataP4ADict instance
  """
  pass
