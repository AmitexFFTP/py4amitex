#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Unit test module for OutReader class.

@author: amarano
"""

import unittest

class TestOutReader(unittest.TestCase):
    """Test Amitex output Reader class."""

    def setUp(self):
        """OutReader tests setup."""
        print('Setting up tests for Amitex OutReader class')

    def test_test(self):
        print("First test.")
        self.assertEqual(4., 4.)

    def test_test2(self):
        print("Second test.")
        self.assertEqual(4., 4.)

class TestOutReader2(unittest.TestCase):
    """Test Amitex output Reader class n°2."""

    def setUp(self):
        """OutReader tests setup."""
        print('Setting up tests for Amitex OutReader class')

    def test_test3(self):
        print("Third test.")
        self.assertEqual(4., 4.)

    def test_test4(self):
        print("Forth test.")
        self.assertEqual(4., 4.)

if __name__ == '__main__':
    unittest.main()