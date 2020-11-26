
### Why HDF5

see https://www.hdfgroup.org/2018/06/hdf5-or-how-i-learned-to-love-data-compression-and-partial-i-o/

book *Python and HDF5*

HDF5 is just about perfect if you make minimal use
of relational features and have a need for
- very high performance,
- partial I/O,
- hierarchical organization,
- and arbitrary metadata.


### Create .pdf file from .md markdown files

try with: https://www.markdowntopdf.com


### Install package py4amitex for developments

For developers, as example **to adapt**.

The problem with using the -e flag for pip install 
is that this requires that the original source directory stay in place 
for as long as you want to use the module. 
It's great if you're a developer working on the source, 
but if you're just trying to install a package, it's the wrong choice.
Alternatively, you don't even need to download the repo from Github at all. 
pip supports installing directly from git repos using a variety of protocols including 
HTTP, HTTPS, and SSH, among others. 


```
# example for developers
export DISTUTILS_DEBUG=yes   # only for developers pip log etc.

# example from USB
export ROOTDIR=/run/media/${USER}/CVW32_1

# example from ROOTDIR as directory where 'git clone ...maisondelasimulation' was done
export ROOTDIR=/volatile2/${USER}

cd ${ROOTDIR}

# as precaution uninstall previous items
rm pip.log                   # as appended logs
# next lines are examples to adapt hardly clean previous pip install
find /volatile/${USER}/miniconda3/envs/py3 -name "py4amitex*" -exec rm -rf {} \;   # example at cea
find /volatile/common/miniconda3/envs/py3 -name "py4amitex*" -exec rm -rf {} \;    # example at home

# choose one of these...
pip install ${ROOTDIR}/py4amitex --log pip.log     # classical install copy files
pip install -e ${ROOTDIR}/py4amitex --log pip.log  # development install, direct improvments in USB

# verification CLI entry_points installation
which LaunchP4A
LaunchP4A --help

# verifications
pluma pip.log
export PY3DIR=/volatile/${USER}/miniconda3/envs/py3  # cea
export PY3DIR=/volatile/common/miniconda3/envs/py3   # home

find ${PY3DIR} -name "py4amitex*"
find ${PY3DIR} -name "py4amitex*" -exec tree {} \;
tree ${PY3DIR}/lib/python3.7/site-packages/py4amitex

ls -alt ${PY3DIR} | head -10
ls -alt ${PY3DIR}/bin | head -10
ls -alt ${PY3DIR}/lib/python3.7/site-packages | head -10
ls -alt ${PY3DIR}/lib/python3.7/site-packages/py4amitex*
```

#### Get PyQt5 version(s)

```
import inspect
from PyQt5 import Qt

vers = ['%s = %s' % (k,v) for k,v in vars(Qt).items() if k.lower().find('version') >= 0 and not inspect.isbuiltin(v)]
print('\n'.join(sorted(vers)))
  PYQT_VERSION = 329986
  PYQT_VERSION_STR = 5.9.2
  QOpenGLVersionProfile = <class 'PyQt5.QtGui.QOpenGLVersionProfile'>
  QOperatingSystemVersion = <class 'PyQt5.QtCore.QOperatingSystemVersion'>
  QT_VERSION = 329991
  QT_VERSION_STR = 5.9.7
```


#### Launch unittest

```
py4amitex command launch test(s).
Launch python unittest (short time) and/or integration tests (more long time)
   
   usage: LaunchP4A [-c cmdName] [-v logLevel] [-w dirName] [-h] [-d]
                    [-p filePattern]
   
   command line interface for py4amitex --cmd test stuff
   
   optional arguments:
     -c cmdName, --cmd cmdName
                           set command to proceed: [bug|sphere_in_cube|tea|test]
                           default=NONE
     -v logLevel, --verbose logLevel
                           set log level verbosity:
                           [CRITICAL|ERROR|WARNING|INFO|DEBUG] default=INFO
     -w dirName, --workdir dirName
                           current working directory
                           default=/volatile/home/christian/PY4AMITEX_WORKDIR
     -h, --help            show this help message (and do no exit)
     -d, --doc             show py4amitex documentation
     -p filePattern, --pattern filePattern
                           file pattern for unittest files
                           ['test_*.py'|'*Test.py'...]
           

LaunchP4A --cmd test

# one test, for example
LaunchP4A --cmd test --pattern="test_4??_*py"

# or directly with knowing path
/volatile2/christian/py4amitex/py4amitex/hdf5py/test/test_400_h5py.py
```


### Install package py4amitex, other pip installation from tar.gz

Firstly create tar.gz, then install.

```
# create tar.gz
cd ${ROOTDIR}/py4amitex
rm -rf dist setup.log py4amitex.egg-info
python setup.py sdist --dist-dir dist

rm ../pip.log
pip install ../dist/py4amitex-0.0.1.tar.gz --log ../pip.log

# other installation from tar.gz
pip install /run/media/${USER}/CVW32_1/py4amitex/dist/py4amitex-0.0.1.tar.gz --log pip.log
```


### Set html doc in python package

Have to set in MANIFEST.in

```
# doc html
recursive-include py4amitex *.html
recursive-include py4amitex *.rst.txt
recursive-include py4amitex *.css
recursive-include py4amitex *.js
```

And verify after make html of course

```
cd ${ROOTDIR}/py4amitex/doc
rm -rf ./py4amitex/doc/build
make html

cd ${ROOTDIR}/py4amitex
rm -rf ./py4amitex/py4amitex/doc
cp -r ./py4amitex/doc/build ./py4amitex/py4amitex/doc

# test user install
rm pip.log ; pip install ./py4amitex --log pip.log
ls -alt ${PY3DIR} | head -10
ls -alt ${PY3DIR}/bin | head -10
ls -alt ${PY3DIR}/lib/python3.7/site-packages | head -10
tree ${PY3DIR}/lib/python3.7/site-packages/py4amitex*


# test doc html, missed style ?
firefox ${PY3DIR}/lib/python3.7/site-packages/py4amitex/doc/html/index.html &
firefox ./py4amitex/doc/build/html/index.html &

```


### Obsolete info for create python package

Example from python-sample-package-with-data

```
cd /run/media/${USER}/CVW32_1
git clone https://github.com/cirosantilli/python-sample-package-with-data.git
rm pip.log ; pip install python-sample-package-with-data --log pip.log

export PY3DIR=/volatile/${USER}/miniconda3/envs/py3 # cea
export PY3DIR=/volatile/common/miniconda3/envs/py3  # maison

ls -alt ${PY3DIR} | head -10
ls -alt ${PY3DIR}/bin | head -10
ls -alt ${PY3DIR}/lib/python3.7/site-packages | head -10
tree ${PY3DIR}/lib/python3.7/site-packages/python_sample_package*
```