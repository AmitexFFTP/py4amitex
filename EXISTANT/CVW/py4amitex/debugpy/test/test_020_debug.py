#!/usr/bin/env python
#-*- coding:utf-8 -*-

#  Copyright (C) 2010-2018  CEA/DEN
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

import os
import sys
import platform
import unittest

# import test.initializeTest # set PATH etc for test

class TestCase(unittest.TestCase):
  "Test the debug.py"
  
  def test_000(self):
    # one shot setUp() for this TestCase
    # DBG.push_debug(True)
    # SAT.setNotLocale() # test english
    return
    
  def test_005(self):
    import py4amitex.debugpy.debug as DBG # Easy print stderr (for DEBUG only)
    res = DBG.getLocalEnv()
    self.assertTrue(len(res.split()) > 0)
    self.assertTrue("USERNAME :" in res)
    if platform.system() != "Windows":
      self.assertTrue("LANG :" in res)
       
  # TODO another test without pyconf
      
  def test_999(self):
    # one shot tearDown() for this TestCase
    # SAT.setLocale() # end test english
    # DBG.pop_debug()
    return
    
if __name__ == '__main__':
    unittest.main(exit=False)
    pass

