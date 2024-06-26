BESOIN UTILISATEUR

Cet outil doit permettre de lancer une simulation amitex depuis un script python sans ecrire un seul fichier xml ou vtk et de réimporter dans python l'ensemble des sorties du code.

CONTRAINTES

* Simplicité du code : afin de faciliter la maintenance ou le développement par 'tous'
* Limiter les prérequis : si possible uniquement les + classiques (numpy, matplotlib etc...) afin de simplifier l'utilisation

IDEE

* Definir un objet par fichier xml (algo.xml, load.xml, mate.xml) et des moyens de générer les fichiers xml à partir de chacun ces objets 

AVANT DE COMMENCER

Dans le répertoire EXISTANT l'ensemble des développement déjà réalisés sur lesquels on pourra ou non s'appuyer pour mettre en place cet outil (travaux de Marc Josien, Aldo Marano, Christian VW et Christophe Bourcier)

## Lauching tests

*py4amitex* tests are implemented into test modules, stored within each subpackage,
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
