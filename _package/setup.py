"""
********************************************************************************
* Name: setup.py
* Author: Gage Larsen
* Created On: May 6th, 2019
* Copyright: (c)
* License: BSD 2-Clause
********************************************************************************
"""
import os
from setuptools import setup, find_packages


# allow setup.py to be run from any path
os.chdir(os.path.normpath(os.path.join(os.path.abspath(__file__), os.pardir)))

requires = [
    'numpy', 'xmscore', 'xmsgrid', 'xmsinterp',
]

version = '1.0.0'

setup(
    python_requires='==3.6.*',
    name='xmsmesher',
    version=version,
    packages=find_packages(),
    include_package_data=True,
    license='BSD 2-Clause License',
    description='',
    author='Aquaveo',
    install_requires=requires,
    package_data={'': ['*.pyd']},
)
