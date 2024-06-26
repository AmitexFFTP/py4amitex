#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
P4A Module to read, write and manipulate input/output files for/from 
AMITEX_FFTP. 

@author: amarano
"""

import struct

# Method to write txt files for per zone material data 
# TODO: adapt methods to various types of data
def write_txt_mat_data(filename="data.txt", data=None):
    # open file 
    File = open(filename, 'w')
    # write data (only floats supported for now)
    for d in data:
        File.write(f'{d:f}\n')
    # close file 
    File.close()

# Method to write binary files for per zone material data 
def write_bin_mat_data(filename="data.bin", data=None):
    # open file and write header
    File = open(filename, 'w')
    File.write(f'{len(data):d}\ndouble\n')
    File.close()
    # open file in binary mode and write data
    File = open(filename, 'ab')
    for d in data:
        # write float data in big endian ordering
        File.write(struct.pack('>d',d))
    # close file
    File.close()