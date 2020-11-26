

### DataP4A (Data Python For Application-s).


Est une classe python.

DataP4A permet: 

- d'initialiser/stocker/modifier des données
- de sauvegarder/sérialiser, restaurer/désérialiser des données
  indépendamment de l'OS et du matériel (du big/little-endian),
  fichiers compressées ou non.
- de gérer/modifier des données utilisateur.
- de utiliser/modifier des données internes au programme.
- de **vérifier** ces données (utilisation de JSON-SCHEMA).
- d'afficher/imprimer directement **lisiblement** ces données (pour debug et production).


Quels types de données:

- simples 
- plus complexes, avec une structure d'arbre,
- avec des tableaux, même de gros tableaux homogènes (numpy)
- avec des tableaux, même de gros tableaux hétérogènes (pandas), 
  cela commence a ressembler à une base de données? oui, un debut.


Quels formats de fichiers:

- Tant qu'il n'y a pas de tableaux/array ,
  on *peut se contenter* du (simple) format fichier JSON (ascii)
- Dès qu'il y a un tableaux/array, 
  on utilise le format fichier HDF5 (binaire, compressé ou pas)

  
Tout ceci correspond à un besoin informatique *courant*.

DataP4A est un moyen de satisfaire ce besoin avec des prérequis python habituels:

- JSON: https://la-cascade.io/json-pour-les-debutants/
- JSON-SCHEMA: https://json-schema.org/learn/getting-started-step-by-step.html
- HDF5: https://deusyss.developpez.com/tutoriels/Python/hdf5
- NUMPY: https://courspython.com/apprendre-numpy.html
- PANDAS: http://www.python-simple.com/python-pandas/panda-intro.php


### Présentation utilisation élémentaire.


Premier exemple

```python
import ...DataP4A as DP4A
mes_data = DP4A.DataP4A()
print('-> mes_data =', mes_data)

-> mes_data = DataP4ADict({})
```

Cette instance DataP4A *ressemble* à un dictionnaire python, 
vide pour l'instant.
En effet la classe *hérite de* `dict`.



### Quelques choses en plus.


Parce que dict est *insuffisant*.


#### DataP4A sait lire un fichier JSON. 


Methode `loadFileJson()`

DataP4A accepte **tout** ce que le format JSON connait, *évidemment*:

- booléen, entier, flottant, string, None, *list, dict*.

```json
fichier data_1.json:
{
  "name": "Tintin",
  "job": "reporter"
}
```

```python
mes_data = DP4A.DataP4A()
mes_data.loadFileJson("data_1.json")
print('-> mes_data =', mes_data)

-> mes_data = DataP4ADict({'name': DataP4AStr('Tintin'), 'job': DataP4AStr('reporter')})
```

- La liste des clés du pseudo dictionnaire **est aussi** 
  la liste des noms des attributs de la classe. 
  
  **Quelle écriture préférez-vous ?**

```python

# oui, quelle ecriture préférez-vous ?

# celle-ci
print(mes_data['name'])
print(mes_data['job'])
-> Tintin
-> reporter

# ou celle-la
print(mes_data.name)
print(mes_data.job)
-> Tintin
-> reporter
```


#### DataP4A sait lire une structure de donnés arborescente (arbre).


Contenant des listes et en afficher/imprimer *lisiblement* le contenu (dans le format asci JSON).
  
Methode `dumpStrJson()`


```json
fichier data_2.json:
{
  "heroes": [
    { "name": "Tintin", "job": "reporter", "age": 30 },
    { "name": "Haddock", "job": "capitaine", "age": 60 }
  ]
}
```

```python
mes_data = DP4A.DataP4A()
mes_data.loadFileJson("data_2.json")
print('-> mes_data =\n%s' % mes_data.dumpStrJson())

-> mes_data =
{
  "heroes": [
    {
      "name": "Tintin",
      "job": "reporter",
      "age": 30
    },
    {
      "name": "Haddock",
      "job": "capitaine",
      "age": 60
    }
  ]
}
```


#### DataP4A accepte les modifications.


Les remplacement, ainsi que les opérations arithmétiques sur les scalaires.
Dans un script python.

  
```python

# un peu de parité (c'est difficile avec hergé)
# notez que l'on ajoute un dictionnaire python !directement!

# soyons galant
une_femme = {"name": "castafiore", "job": "cantatrice", "age": None}
mes_data.heroes.append(une_femme)

haddock = mes_data.heroes[1]
haddock.name = "Karpock"  # modification
hddock.age -= 5           # opérations arithmétiques

my_print('mes_data =\n%s' % mes_data.dumpStrJson())

-> mes_data =
{
  "heroes": [
    {
      "name": "Tintin",
      "job": "reporter",
      "age": 30
    },
    {
      "name": "Karpock",
      "job": "capitaine",
      "age": 55
    },
    {
      "name": "castafiore",
      "job": "cantatrice",
      "age": null
    }
  ]
}
```


#### DataP4A accepte les gros tableaux. 


Tableaux numpy *ndarrays*, tableaux pandas *dataframes*.


```python

mes_data = DP4A.DataP4A()
mes_data.loadFileJson("data_2.json")

# ajout d'un noeud/branche 'some_arrays' a l'arbre
mes_data.some_arrays = DP4A.DataP4A()

nb = 10000
# i.e. ajout d'une feuille 'array_one' à l'arbre
mes_data.some_arrays.array_one = np.ones((nb, nb))

# i.e. ajout d'une feuille 'array_two' à l'arbre
aDataFrame = pd.DataFrame({'AA': [11, 22, 33], 'BB': [44, 55., 66]})
mes_data.some_arrays.array_two = aDataFrame

# dumpStrJson() n'affiche pas le contenu des arrays.
# Json n'est vraiment pas prevu pour cela
my_print('mes_data =\n%s' % mes_data.dumpStrJson())

-> mes_data =
{
  "heroes": [
    {
      "name": "Tintin",
      "job": "reporter",
      "age": 30
    },
    {
      "name": "Haddock",
      "job": "capitaine",
      "age": 60
    }
  ],
  "some_arrays": {
    "array_one": "ndarray(10000, 10000)",
    "array_two": "dataframe(3, 2)"
  }
}

# dumpFileHdf5() cree un fichier hdf5, n'oublie pas les arrays, c'est LE PLUS.
mes_data.dumpFileHdf5("data_2_modified.hdf5")

# loadFileHdf5() relit un fichier hdf5, n'oublie pas les arrays, oeuf corse.
mes_data_modified = DP4A.DataP4A()

# verbose=True sort (optionnellement) un log-resumé du contenu du fichier.
mes_data.loadFileHdf5("data_2_modified.hdf5", verbose=True)
```


```text

Le log-resumé:
->

[INFO    ] meta_informations/info_hdf5_file:
           {
             "version": "1.0.0",
             "file_name_origin": "data_2_modified.hdf5",
             "hostname_origin": "is231761",
             "username_origin": "christian",
             "file_date": "21/11/2020 12:54:52"
           }
[INFO    ] data/data_json:
           {
             "heroes": [
               {
                 "name": "Tintin",
                 "job": "reporter",
                 "age": 30
               },
               {
                 "name": "Haddock",
                 "job": "capitaine",
                 "age": 60
               }
             ],
             "some_arrays": {
               "array_one": "x000_ndarray_.some_arrays.array_one",
               "array_two": "x001_dataframe_.some_arrays.array_two"
             }
           }
[INFO    ] arrays_paths:
           {'x000_ndarray_': '.some_arrays.array_one',
            'x001_dataframe_': '.some_arrays.array_two'}
[INFO    ] loadFileHdf5 x000_ndarray_ ---> .some_arrays.array_one
           [[1. 1. 1. ... 1. 1. 1.]
            [1. 1. 1. ... 1. 1. 1.]
            [1. 1. 1. ... 1. 1. 1.]
            ...
            [1. 1. 1. ... 1. 1. 1.]
            [1. 1. 1. ... 1. 1. 1.]
            [1. 1. 1. ... 1. 1. 1.]]
[INFO    ] loadFileHdf5 x001_dataframe_ ---> .some_arrays.array_two
              AA    BB
           0  11  44.0
           1  22  55.0
           2  33  66.0
[INFO    ] groupe /arrays:
           array(10000, 10000) '/arrays/x000_ndarray_'   pypath '.some_arrays.array_one'
           array(3, 2) '/arrays/x001_dataframe_' pypath '.some_arrays.array_two'
           

```

#### DataP4A sait sauvegarder les arrays en mode compressé.

 
Ce sont les modes de compression du format fichier HDF5.

Methode `dumpFileHdf5()`


```python
mes_data.dumpFileHdf5("data_2_modified.hdf5", compression=None)
mes_data.dumpFileHdf5("data_2_modified_compressed.hdf5", compression='gzip')
```

Les fichiers résultants ont des tailles différentes.


```bash
ls -alt data_2_modified*hdf5
-rw-r--r-- 1 christian lgls   1848818 22 nov.  11:36 data_2_modified_compressed.hdf5
-rw-r--r-- 1 christian lgls 800013752 22 nov.  11:36 data_2_modified.hdf5
```


#### DataP4A sait vérifier/valider l'arbre.


Utilisation de JSON-SCHEMA.

Methode `json_validate()`

Notez que le schéma dans l'exemple *pourrait* décrire *beaucoup* plus de contraintes, 
sur la structure de l'arbre, 
et/ou sur le type de ses feuilles, 
et/ou les intervalles de validité de ses feuilles (scalaires).

Voir https://json-schema.org/learn/getting-started-step-by-step.html


```python
schema_1 = '''
{ 
  "type": "object", 
  "required": [ "heroes" ]
}
'''

schema_2 = '''
{ 
  "type": "object", 
  "required": [ "heroes", "some_arrays" ]
}
'''

data_to_test = DP4A.DataP4A()

schema_validation = json.loads(schema_1)
data_to_test.loadFileJson("data_2.json")
data_to_test.json_validate(schema_validation)

schema_validation = json.loads(schema_2)
# vous pouvez attraper l'exception levée ici par 'try...except', évidemment.
data_to_test.json_validate(schema_validation)

```

L'exception levée par `...site-packages/jsonschema/validators.py` 
est souvent *assez explicite*:


```text
->

File "...site-packages/jsonschema/validators.py", line 934, in validate
    raise error
jsonschema.exceptions.ValidationError: 'some_arrays' is a required property

Failed validating 'required' in schema:
    {'required': ['heroes', 'some_arrays'], 'type': 'object'}

On instance:
    {'heroes': [{'age': DataP4AInt(30),
                 'job': DataP4AStr('reporter'),
                 'name': DataP4AStr('Tintin')},
                {'age': DataP4AInt(60),
                 'job': DataP4AStr('capitaine'),
                 'name': DataP4AStr('Haddock')}]}

```

#### DataP4A sait parcourir l'arbre.


Comme on peut déjà parcourir en python un `dict`, 
contenant possiblement des `list`,
contenant possiblement d'autres trucs.
 

```python

mes_data = DP4A.DataP4A()
mes_data.loadFileJson("data_2.json")
mes_data.some_arrays = DP4A.DataP4A()

# as for dict
for key, value in mes_data.items():
  print(key)
  
-> heroes
-> some_arrays

# as for list
for hero in mes_data.heroes:
  print(hero)

-> Tintin
-> Haddock
```

#### DataP4A sait remonter l'arbre.


A partir des feuilles (ou d'une branche), sait *(non fortuitement)* déterminer 
ce qu'on nommera le **python path** d'un item de l'arbre. 

Car **Un item DataP4A (branche/feuille) de l'arbre connaît son parent dans l'arbre**.

Methode `getpythonpath()`


```python
mes_data = DP4A.DataP4A()
mes_data.loadFileJson("data_2.json")

a_hero = mes_data.heroes[1]
a_pythonpath = a.hero.getpythonpath()
print("a_pythonpath = %s" % a_pythonpath)

-> a_pythonpath = .heroes[1]
```


#### DataP4A reconnaît AUSSI la syntaxe python.


C'est à dire que la syntaxe JSON **n'est pas le seul moyen de définir 
*from scratch* un arbre de donnée**.

Attention, la différence est subtile, la syntaxe JSON **n'est pas** la syntaxe Python

L'utilisateur est ainsi *libre* de définir ses données dans un fichier à lui 
**en language python** plutôt qu'en JSON.

Ainsi, *contrairement au JSON* les *commentaires sont permis*. 
Tout est permis? Non! Il faut garder à l'esprit que le script doit se limiter 
à une **simple définition de données**.

Il est montré dans cet exemple que *définir un arbre* se 
réduit à *définir un dictionnaire python*, nommé `DATA_IN`, 
*dont les items sont du type acceptés par DataP4A*.
  
Methode `loadFilePY()` `dumpStrPy()`


```python
fichier data_1.py:

# almost everything python code allowed here, but 
# you have to KISS, 
# target is DATA in python dict syntax, not really a program
# these comments are allowed in python syntax, obviously!

import math

# temporary value, all python expressions accepted, it is KISS python script
temp_value = math.pi * 2.

# in python dict duplicate keys select last one without warning
DATA_IN = {
    "pi": math.pi,
    "pi2": temp_value,         # comment accepted...
    "nothing": None,
    "abool": True,
    "long_line": """hello
... one more line from hello ...
""",
    "calculated_value": 1+2.,  # all python expressions accepted...
}
```

```python
# file name could accept absolute file path...
mes_data.loadFilePY("data_1.py")
my_print("mes_data output json\n%s" % mes_data.dumpStrJson())
```

```json
-> mes_data output json:
{
  "pi": 3.141592653589793,
  "pi2": 6.283185307179586,
  "nothing": null,
  "abool": true,
  "long_line": "hello\n... one more line from hello ...\n",
  "calculated_value": 3.0
}
```

```python
mes_data.loadFilePY(data_1.py)
my_print("mes_data output python\n%s" % mes_data.dumpStrPy())
```

```python
-> mes_data output python
{'abool': True,
 'calculated_value': 3.0,
 'long_line': 'hello\n... one more line from hello ...\n',
 'nothing': None,
 'pi': 3.141592653589793,
 'pi2': 6.283185307179586}
```


#### Avantages


- JSON est une passerelle vers Java, et des librairies existent en Python/C/C++/fortran/etc.
- HDF5 est un format *normalisé, pérenne*, utilisé *entre autres* par Salomé, 
  de *nombreuses* librairies existent en Python/C/C++/fortran/etc.
- Il existe *au moins* un viewer HDF5 (GUI Qt) `vitables`.
- Il existe *au moins* un utilitaire HDF5 (asci) `h5dump`.


#### Inconvénients


- Les clés-dictionnaire/attributs-JSON/python utilisées sont **limitées** à 
  la syntaxe de nommage des attributs Python: 
  pas d'espace, pas d'accentuation française, ou autre caractères spéciaux.
   
- DataP4A possède une API, il faut apprendre à l'utiliser proprement, 
  il y a *toujours* des pièges dans une API.
  