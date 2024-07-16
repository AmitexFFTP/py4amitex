#!/bin/bash
VERSION=3.8.19
wget https://www.python.org/ftp/python/$VERSION/Python-$VERSION.tgz
tar -xzvf Python-$VERSION.tgz
cd Python-$VERSION
./configure
make
make install
cd ..
