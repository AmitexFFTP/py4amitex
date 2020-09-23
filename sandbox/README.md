

### Sandbox py4amitex

A directory to keep some tricks, for memory.
With or without explanations.
Use as your own risks.

#### Install python 3 and prerequisites numpy etc. for py4amitex

Try to use API `import paraview.simple`

WARNING: 2020 There is a problem with anaconda paraview, so not included in *py3*.

User may install paraview (with yum etc. as root) (Centos 7 as version 4.4.0)
This is lead to problems on display vtk files version, fix not simple, oops.
One fix could be using vtk xml files, which seems robust. 

Problems:
- vtk files version
- bug driver mesa on usage
- bug 'vtkInitializationHelper' is not defined for recent versions.


```
# on is221713 centos7

# create py3 local without paraview OK
conda create --name py3 \
      python=3.7 pip sphinx matplotlib numpy pandas pandas-datareader pyqt=5.9 jsonschema pyyaml libxml2

# create py27s85pv local as salome 8.5 paraview python 2 old version OK
conda create -c conda-forge --name py27s85pv  python=2.7 paraview=5.4 pip sphinx matplotlib numpy pandas pandas-datareader pyqt=5.9 jsonschema pyyaml libxml2

# create py3 local with paraview as salome 9.4 KO
# this is do not work for paraview API use...
conda create -c conda-forge --name py36s94pv \
      python=3.6.5 pip sphinx matplotlib numpy pandas pandas-datareader pyqt=5.9 jsonschema pyyaml libxml2 paraview 
```

Example of bug, KitWare known it.

```
conda activate py36s94pv
python
    Python 3.6.11 | packaged by conda-forge | (default, Aug  5 2020, 20:09:42) 
    [GCC 7.5.0] on linux
    Type "help", "copyright", "credits" or "license" for more information.
    >>> from paraview import simple as PV
    Error: Cannot import vtkPVServerManagerApplication
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
      File "/volatile/wambeke/miniconda3/envs/py36s94pv/lib/python3.6/site-packages/paraview/simple.py", line 41, in <module>
        from paraview import servermanager
      File "/volatile/wambeke/miniconda3/envs/py36s94pv/lib/python3.6/site-packages/paraview/servermanager.py", line 3233, in <module>
        vtkInitializationHelper.Initialize(sys.executable,
    NameError: name 'vtkInitializationHelper' is not defined
    >>> 
```

#### save py4amited as zip

```
# from usb1 (CVW32_1) to usb2 (CVW16_2)
cd /run/media/${USER}/CVW32_1
zip -r /run/media/${USER}/CVW16_2/py4amitex_201713.zip py4amitex
```

#### save py4amited as zip for ftp.cea.fr

Usage for pip install -e. Create zip file for development.
**without git information**

```
# from usb1 (CVW32_1) to /data/tmplgls
USB1=/run/media/${USER}/CVW32_1
TRG=/data/tmplgls/${USER}/tmp
timestamp=$(date +"%y%m%d")
echo ${timestamp}

mkdir ${TRG}
cd ${TRG}
pwd
rm -rf ${TRG}/py4amitex
cp -r /run/media/${USER}/CVW32_1/py4amitex .
cd py4amitex
git hisc > git_history.txt
rm -rf py4amitex.egg-info
rm -rf .git
find . -name "__pycache__" -type d -exec rm -rf {} \;
cd ${TRG}
rm py4amitex_${timestamp}.zip
zip -r py4amitex_${timestamp}.zip py4amitex

ftp ftp.cea.fr
ftp> cd incoming/y2k01
ftp> mkdir py4amitex
ftp> cd py4amitex
ftp> put py4amitex_200722.zip
ftp> quit
```

#### install py4amited from zip from ftp

pip install development mode, **without git information**

```
ftp ftp.cea.fr
ftp> cd incoming/y2k01/py4amitex
ftp> get py4amitex_200722.zip
ftp> quit

# OR easier...
firefox ftp:\\ftp.cea.fr:/incoming/y2k01/py4amitex/py4amitex_200722.zip

# ...decompress folder py4amitex in UserOwnerFolder in ?local? disk device
cd UserOwnerFolder
pip install -e ./py4amitex
```


#### Test pip install

The problem with using the -e flag for pip install 
is that this requires that the original source directory stay in place 
for as long as you want to use the module. 
It's great if you're a developer working on the source, 
but if you're just trying to install a package, it's the wrong choice.
Alternatively, you don't even need to download the repo from Github at all. 
pip supports installing directly from git repos using a variety of protocols including 
HTTP, HTTPS, and SSH, among others. 


##### Test pip install dev mode

For example

```
pip install --help

which python
  /volatile/wambeke/miniconda3/envs/py3/bin/python
which pip
  /volatile/wambeke/miniconda3/envs/py3/bin/pip
find /volatile/wambeke/miniconda3/envs/py3 -type d -name site-packages
  /volatile/wambeke/miniconda3/envs/py3/lib/python3.7/site-packages

export SITE=/volatile/wambeke/miniconda3/envs/py3/lib/python3.7/site-packages
ls -alt ${SITE} | grep py4amitex

export USB1=/run/media/${USER}/CVW32_1
ls -alt ${USB1}

# install dev mode as link to ${USB1}/py4amitex
pip install -e ${USB1}/py4amitex
    Obtaining file:///run/media/wambeke/CVW32_1/py4amitex
    Installing collected packages: py4amitex-wambeke
      Attempting uninstall: py4amitex-wambeke
        Found existing installation: py4amitex-wambeke 0.0.1
        Uninstalling py4amitex-wambeke-0.0.1:
          Successfully uninstalled py4amitex-wambeke-0.0.1
      Running setup.py develop for py4amitex-wambeke
    Successfully installed py4amitex-wambeke

ls -alt ${SITE} | grep py4amitex
   -rw-r--r--  1 wambeke lgls     38  7 juil. 16:36 py4amitex-wambeke.egg-link
cat ${SITE}/py4amitex* # is a link to ${USB1}/py4amitex
   /run/media/wambeke/CVW32_1/py4amitex
```

##### Test pip install prod mode

Install production mode as duplicate ${USB1}/py4amitex in folder ${SITE}/py4amitex

```
export PY3DIR=$(dirname $(dirname $(which python)))
echo ${PY3DIR}

export USB1=/run/media/${USER}/CVW32_1
echo ${USB1}

export SITE=${PY3DIR}/lib/python3.7/site-packages

cd ${USB1}
pip install ${USB1}/py4amitex
ls -alt ${SITE} | grep py4amitex
    drwxr-xr-x  2 christian lgls   4096 14 juil. 17:04 py4amitex-0.0.1.dist-info
    drwxr-xr-x 10 christian lgls   4096 14 juil. 17:04 py4amitex
    -rw-r--r--  1 christian lgls     40 13 juil. 11:04 py4amitex.egg-link

tree ${SITE}/py4amitex*
    /volatile/wambeke/miniconda3/envs/py3/lib/python3.7/site-packages/py4amitex
    ├── __init__.py
    └── __pycache__
        └── __init__.cpython-37.pyc
    /volatile/wambeke/miniconda3/envs/py3/lib/python3.7/site-packages/py4amitex_wambeke-0.0.1.dist-info
    ├── direct_url.json
    ├── INSTALLER
    ├── LICENCE_AMITEX.pdf
    ├── METADATA
    ├── RECORD
    ├── top_level.txt
    └── WHEEL

```


### Save zip files to CVW16_2

```
# on pc at home
cd /volatile2/home/christian
USB=/run/media/${USER}/CVW16_2
zip -r ${USB}/py4amitex_200702.zip py4amitex
zip -r ${USB}/py4amitex_200702.zip py4amitex
zip -r ${USB}/neper_github_340_composite_200702.zip neper_github_340_composite
zip -r ${USB}/CMDC_lgls_200702.zip CMDC_lgls

zip ${USB}/READMES_200702.zip ~/README*
```

### Restore zip files from CVW16_2

```
# on pc at cea from usb to usb
USBS=/run/media/${USER}/CVW16_2     # source
USBT=/run/media/${USER}/CVW32_1     # target
cd ${USBT}
unzip ${USBS}/py4amitex_200702.zip
unzip ${USBS}/py4amitex_200707.zip
unzip ${USBS}/CMDC_lgls_200707.zip
```

### get *code* amitex_fftp from maisondelasimulation

Have to logging before `firefox user https://gitlab.maisondelasimulation.fr`
user christian.van-wambeke@cea.fr

```
USBT=/run/media/${USER}/CVW32_1     # target
USBS=/run/media/${USER}/CVW16_2     # source

cd ${USBT}
git clone https://gitlab.maisondelasimulation.fr/jderouil/amitex_fftp.git
zip -r ${USBS}/amitex_fftp_200707.zip amitex_fftp
```
