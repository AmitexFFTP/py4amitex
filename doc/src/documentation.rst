
.. include:: ../rst_prolog.rst

.. _iraDocumentation:

************************************
Documentation
************************************

.. _iraDocumentation_consultation:


Doc consultation
=================

To display py4amitex html documentation in your web browser *firefox*, or *else*.
The initial entry file is located at *py4amitex/doc/index.html*.

.. code-block:: bash

  # Linux bash, as an example
  cd .../py4amitex
  firefox doc/index.html &
  # or as CLI
  LaunchP4A --doc
  
.. _iraDocumentation_modification:

Doc modification
=================

To modify py4amitex SPHINX_ documentation with simple editor *pluma*, or else.

Read the manual, see http://www.sphinx-doc.org/en/stable/tutorial.html,
add or modify files in **development repository (with git)** *py4amitex/py4amitex/doc/src/xxxx.rst*.

.. code-block:: bash

  # Linux bash, as an example
  cd ...py4amitex/
  tmp=$(find doc -name "*.rst")
  pluma $tmp &


.. _iraDocumentation_compilation:

Doc compilation Linux
======================

On a Linux system, to compile py4amitex html documentation,
in **development repository (with git)**.
Programmers use installed GNU_ *make*, and SPHINX_.

.. warning:: To make documentation **pdf** programmers needs
             installed *texlive* package (preferably up to date version).
             See: https://www.tug.org/texlive/quickinstall.html


.. code-block:: text

  cd ...py4amitex/doc
  cat README  # read some environment setup information
  #  ... and read it
  make
    Please use `make <target>' where <target> is one of
    html       to make standalone HTML files
    dirhtml    to make HTML files named index.html in directories
    singlehtml to make a single large HTML file
    pickle     to make pickle files
    json       to make JSON files
    htmlhelp   to make HTML files and a HTML help project
    qthelp     to make HTML files and a qthelp project
    devhelp    to make HTML files and a Devhelp project
    epub       to make an epub
    latex      to make LaTeX files, you can set PAPER=a4 or PAPER=letter
    latexpdf   to make LaTeX files and run them through pdflatex
    text       to make text files
    man        to make manual pages
    changes    to make an overview of all changed/added/deprecated items
    linkcheck  to check all external links for integrity
    doctest    to run all doctests embedded in the documentation (if enabled)
  
  # and then
  make html      # make build/html (.html files)
  make latexpdf  # make build/latex (.pdf file)



