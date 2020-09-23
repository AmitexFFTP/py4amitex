

### py4amitex package

Python utilities for amitex_fftp code,  
see https://gitlab.maisondelasimulation.fr/jderouil/amitex_fftp

*Designed for python V3.7 as miniconda py3*

User have to get a correct python environment, with good python prerequisites (numpy etc.)


### Fristly Install python 3.7 and prerequisites for py4amitex

For example, use *miniconda3*.

Create a user local python 3.7 valid for py4amitex,
see https://conda.io/en/latest/miniconda.html 


```
# 2020

bash .../Miniconda3-latest-Linux-x86_64.sh
...etc...
# https://anaconda.org/conda-forge/paraview

# create py3 local, with some usual prerequisites
conda create --name py3 \
      python=3.7 pip sphinx matplotlib numpy pandas pandas-datareader pyqt=5.9 jsonschema pyyaml libxml2

# verify it is ok
conda activate py3
which python
which pip
```

### Install package py4amitex simple usage
 
Get folder py4amitex from maisondelasimulation, and use **pip**.

https://gitlab.maisondelasimulation.fr/jderouil/py4amitex
 
```
# install from local folder 
conda activate py3
pip install ./py4amitex
```

