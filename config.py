#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os

# Pathes towards amitex_fftp repository
# TO SET MANUALLY
AMITEX_DIR = os.getenv("AMITEX_PATH", "/stck/amarano/Codes/amitex_fftp")  # amitex main directory
AMITEX_TEST_RES_DIR = os.path.join(AMITEX_DIR, "resultats") # test case results


py4amitex_ROOT = os.path.dirname(os.path.abspath(__file__)) # py4amitex root directory
py4amitex_EXAMPLES_DIR = os.path.join(py4amitex_ROOT, 'examples') # py4amitex examples dir
