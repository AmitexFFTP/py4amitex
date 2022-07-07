
.. include:: ../rst_prolog.rst

.. _iraInstallation:

**************************
Development installations
**************************

A development installation of py4amitex allows programmer improvments.
It is a classical usage of Python_ packages.



.. _iraInstallation_pythonlinux:

Python installation Linux
============================

To install python3 (and its mandatory packages numpy etc.) *locally*, we suggest to use miniconda_.
Note that *miniconda* is windows7-10 compliant.

.. note:: You may use this Python interpreter for another python scripting code than py4amitex.

For information:

#. https://conda.io/miniconda.html
#. https://conda.io/docs/index.html

Example of install *(Linux-bash)*:

.. code-block:: bash

    bash Miniconda3-latest-Linux-x86_64.sh
    # -> Miniconda3 will now be installed into this location:
    # -> /volatile/common/miniconda3 (for example. It is located as you want.)
    # -> Thank you for installing Miniconda3!

    export PATH=/volatile/common/miniconda3/bin:$PATH
    which conda
    # -> /volatile/common/miniconda3/bin/conda

    conda create --name py3 python=3.7 \
       pip sphinx jupyter matplotlib numpy \
       pandas pandas-datareader \
       jsonschema pyyaml libxml2 paramiko
    # -> Solving environment: done
    # -> Proceed ([y]/n)? y
    # -> Downloading and Extracting Packages
    # -> To activate this environment, use:
    # -> conda activate py3

    conda info --envs
    # -> conda environments:
    # ->   base         /volatile/common/miniconda3
    # ->   py3          /volatile/common/miniconda3/envs/py3
    # ->   etc...

    conda activate py3
    which python
    # - > /volatile/common/miniconda3/envs/py3/bin/python


.. _iraInstallation_linux:

py4amitex installation Linux
==============================


.. warning:: Python interpreter py3 is supposed to be set
             and useful in environment path.
             Usually command *conda activate py3* assume that.

Example of install/launch *(Linux-bash)*:

.. code-block:: bash

    cd whereYouwant
    git clone .../py4amitex.git    # from maisondelasimulation
    which python                   # --> py3
    pip install ./py4amitex

    # launch as API ...
    python
    >> import py4amitex as P4A
    >> etc...

    # ... or launch as CLI
    LaunchP4A --help                 # on line help





