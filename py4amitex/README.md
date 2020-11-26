

### Package py4amitex

Python utilities for *amitex_fftp code*, is a `pip` package,  
*Designed for python V3.7 as miniconda py3*


### Fristly Install python 3.7 and prerequisites for py4amitex

For example, use *miniconda3*.

Create a user local python 3.7 valid for py4amitex,
see https://conda.io/en/latest/miniconda.html


```
# 2020

bash .../Miniconda3-latest-Linux-x86_64.sh
...etc...

y
# create py3 local, with some usual prerequisites
conda create --name py3 \
      python=3.7 pip sphinx matplotlib numpy pandas pandas-datareader pyqt=5 jsonschema pyyaml libxml2 h5py vtk

conda create  -c conda-forge --name py3b \
      python=3.7 pip sphinx matplotlib numpy pandas pandas-datareader pyqt=5 jsonschema pyyaml libxml2 h5py vtk vitables=3.0.2

# verify it is ok
conda activate py3
which python
which pip
```

may be

```
WARNING: A newer version of conda exists. <==
  current version: 4.9.1
  latest version: 4.9.2

Please update conda by running

    $ conda update -n base -c defaults conda
```


If you really need paraview, install *optionally* paraview.
See https://anaconda.org/conda-forge/paraview. 
Or use sytem-installed (as root) paraview.

```
which paraview
  /usr/bin/paraview
```

If you really need vitables, install *optionally* vitables.
See https://pypi.org/project/ViTables for comprehension.

WARNING: sometimes install one package with pip, the other with conda, is risky.

```
conda install -c conda-forge vitables  # version 3.0.0
# or...
pip3 install vitables # version 3.0.2

    Collecting vitables
      Downloading ViTables-3.0.2-py3-none-any.whl (1.0 MB)
         |████████████████████████████████| 1.0 MB 6.5 MB/s 
    Requirement already satisfied: numpy>=1.4.1 in /volatile/common/miniconda3/envs/py3/lib/python3.7/site-packages (from vitables) (1.19.2)
    Collecting tables>=3.0
      Downloading tables-3.6.1-cp37-cp37m-manylinux1_x86_64.whl (4.3 MB)
         |████████████████████████████████| 4.3 MB 42.4 MB/s 
    Collecting PyQt5>=5.5.1
      Downloading PyQt5-5.15.1-5.15.1-cp35.cp36.cp37.cp38.cp39-abi3-manylinux2014_x86_64.whl (71.6 MB)
         |████████████████████████████████| 71.6 MB 48 kB/s 
    Collecting numexpr>=2.0
      Downloading numexpr-2.7.1-cp37-cp37m-manylinux1_x86_64.whl (162 kB)
         |████████████████████████████████| 162 kB 87.0 MB/s 
    Collecting qtpy>=1.2.1
      Downloading QtPy-1.9.0-py2.py3-none-any.whl (54 kB)
         |████████████████████████████████| 54 kB 592 kB/s 
    Collecting PyQt5-sip<13,>=12.8
      Downloading PyQt5_sip-12.8.1-cp37-cp37m-manylinux1_x86_64.whl (283 kB)
         |████████████████████████████████| 283 kB 76.0 MB/s 
    Installing collected packages: numexpr, tables, PyQt5-sip, PyQt5, qtpy, vitables
 
# fix vitables error when display dataset AttributeError: module 'pandas' has no attribute 'plotting'
pip3 install pandas -U
  Installing collected packages: pandas
    Attempting uninstall: pandas
      Found existing installation: pandas 1.1.3
      Uninstalling pandas-1.1.3:
        Successfully uninstalled pandas-1.1.3
  Successfully installed pandas-1.1.4
```


### Install package py4amitex simple usage
 
Get folder py4amitex from maisondelasimulation, and use **pip**.

 
```
cd YourChoiceDirectory
git clone https://gitlab.maisondelasimulation.fr/GELEBART_LIONEL/py4amitex.git
# install from local folder 
conda activate py3
pip install ./py4amitex
```

