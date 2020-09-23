
.. include:: ../rst_prolog.rst


.. _iraUsage:

********************
Usage of py4amitex
********************

Usage
=====

py4amitex usage is a Command Line Interface (CLI_), which is
Windows *and* Linux compatible.

.. code-block:: bash

  LaunchP4A --[options]


Options of LaunchP4A
............................

Useful but *not exhaustive* generic options of *LaunchP4A* CLI_.


Option *help or h*
............................

Get help as simple text.

.. code-block:: bash

    LaunchP4A --help          # get list of existing options


Option *doc or d*
............................

Get documentation as browser html.

.. code-block:: bash

    LaunchP4A --doc           # see html doc


Option *verbose or v*
............................

Change verbosity level (default is 'info').

.. code-block:: bash

    # execute LaunchP4A command in verbose debug mode
    LaunchP4A -v debug


Option *workdir or w*
............................

Change working directory (user data directory). Default is
${HOME}/PY4AMITEX_WORKDIR

.. code-block:: bash

    # execute LaunchP4A in user choice working directory
    LaunchP4A -w .../MY_WORKDIR



