

### Create py4amitex git from scratch

Read https://packaging.python.org/tutorials/packaging-projects/

Get example pypa from setuptools and make it simplest
from https://github.com/pypa/setuptools

```
cd /volatile2/christian/tmp
unzip ~/Téléchargements/setuptools-master.zip
```

#### Create py4amitex simplest 

And create/copy firsts files and git init.

```
mkdir /volatile/home/christian/py4amitex
cd /volatile/home/christian/py4amitex
git init
...
git commit -m "first commit"
  [master (root-commit) b52941c] first commit
   6 files changed, 62 insertions(+)
   create mode 100644 .gitignore
   create mode 100644 README.md
   create mode 100644 doc/README.md
   create mode 100644 py4amitex/README.md
   create mode 100644 py4amitex/__init__.py
   create mode 100644 sandbox/README.md
git hisc
* b52941c - (HEAD, master) first commit (11 minutes ago) <Christian Van Wambeke>
```

#### Clone py4amitex
 
Clone on other device and work in it.

```
cd /volatile2/christian
git clone /volatile/home/christian/py4amitex
...
git push

```

