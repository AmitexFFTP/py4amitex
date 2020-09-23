

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
# example for developers from USB
export DISTUTILS_DEBUG=yes   # only for developers pip log etc.
export USB1=/run/media/${USER}/CVW32_1
cd $USB1

# as precaution uninstall previous items
rm pip.log                   # as appended logs
find /volatile/${USER}/miniconda3/envs/py3 -name "py4amitex*" -exec rm -rf {} \;   # cea
find /volatile/common/miniconda3/envs/py3 -name "py4amitex*" -exec rm -rf {} \;   # home

# choose one of these...
pip install ${USB1}/py4amitex --log pip.log     # classical install copy files
pip install -e ${USB1}/py4amitex --log pip.log  # development install, direct improvments in USB

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


### Install package py4amitex, other pip installation from tar.gz

Firstly create tar.gz, then install.

```
# create tar.gz
cd $USB1/py4amitex
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
cd $USB1/py4amitex/doc
rm -rf ./py4amitex/doc/build
make html

cd $USB1/py4amitex
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