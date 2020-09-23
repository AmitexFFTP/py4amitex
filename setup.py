#!/usr/bin/env python

"""
Distutils setup file, used to install or test 'py4amitex'
"""

import os
# import sys
import setuptools
import pprint as PP

# here = os.path.abspath(os.path.dirname(__file__))
here = os.path.dirname(__file__)
print("here is %s" % here)

user = os.getenv('USER')

#with open(os.path.join(here, "README.md", "r")) as fh:
#    long_description = fh.read()
long_description = "TODOOOOOOO"

if True: #user is None:
    name_package = "py4amitex"
else:
    name_package = "py4amitex-" + user # Completed with your own username

print("setuptools.find_packages() are\n%s" % PP.pformat(setuptools.find_packages()))
#print("setuptools.locals() are\n%s" % PP.pformat(locals()))
#print("setuptools.globals() are\n%s" % PP.pformat(globals()))

setup_params = dict(
    name=name_package,
    version="0.0.1",
    author="Christian Van Wambeke",
    author_email="christian.van-wambeke@cea.fr",
    description="python utilities for amitex_fftp code",
    long_description=long_description,
    long_description_content_type="text/markdown",
    # url="https://gitlab.maisondelasimulation.fr/jderouil/py4amitex",
    # "https://github.com/pypa/py4amitex",
    packages=setuptools.find_packages(),
    include_package_data=True, # use MANIFEST.in
    # package_dir={'': 'doc'},
    # package_data={'': ['*']},
    xxxdata_files=[
        ('py4amitex_data',   # or 'lib/python3.7/site-packages/py4amitex/py4amitex_data',
            [
            'LICENCE_AMITEX.pdf',
            'LINCE_AMITEX.txt',
            'README.md',
            #'./doc/buid/latex/py4amitex.pdf',
            ],
        ),
    ],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: LICENCE_AMITEX",
        "Operating System :: Linux", # OS Independent"
    ],
    python_requires='>=3.6, <4',
    entry_points={
        'console_scripts': [
            'LaunchP4A=py4amitex.amitexpy.LaunchP4A:run',
         ],
    },

)

if __name__ == '__main__':
    # allow setup.py to run from another directory
    # here and os.chdir(here)
    dist = setuptools.setup(**setup_params)
    # print("dist is\n", dist)
