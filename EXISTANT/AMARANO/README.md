
## Package p4am : Aldo Marano's preliminary version of Py4amitex Python package

This sub-directory of py4amitex project is an individual attempt to
produce a draft version of the future Py4amitex Python package, aiming
to launch a simulation from a script without writing a .vtk or .xml file.

Contact information : Aldo Marano aldo.marano@onera.fr

## Python Environment 

To create a suitable environment to use *p4am* with Anaconda : 

`conda env create -f environment.yml` 

## Current content

TO COMPLETE

See `examples` directory for scripts that illustrates the use of P4AM to
postprocess the results of the validation test cases of AMITEX_FFTP.

## Lauching tests

*p4am* tests are implemented into test modules, stored within each subpackage,
in a `tests` directory. All tests are implemented as python `unittest` classes.
To launch the tests you can use the `unittest` test launcher:

- run `python -m unittest test_file` in the appropriate `tests` directory to
  run all tests implemented within the file `test_file.py`.

- run `python -m unittest test_file.test_class` in the appropriate `tests`
  directory to run all tests implemented within the class `test_class` of the
  file `test_file.py`.

- run `python -m unittest test_file.test_class.test_case` in the appropriate
  `tests` directory to run the test `test_case` implemented within the class
  `test_class` of the file `test_file.py`.

- run `python -m unittest discovery` in the appropriate `tests` directory to
  to run all tests implemented in the directory
