#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
This file contains simple utilities for py4amitex
"""

import os

def getWorkdirDefault():
  res = os.getenv("PY4AMITEX_WORKDIR")
  if res is None:
    res = "${HOME}/PY4AMITEX_WORKDIR"
  res = os.path.realpath(os.path.expandvars(res))
  return res

os.environ["PY4AMITEX_WORKDIR"] = getWorkdirDefault()


