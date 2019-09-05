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
from setuptools import setup
from xms.mesher import __version__


# allow setup.py to be run from any path
os.chdir(os.path.normpath(os.path.join(os.path.abspath(__file__), os.pardir)))

requires = [
    'numpy',
    'xmscore>=3.0.5',
    'xmsgrid>=4.1.0',
    'xmsinterp>=3.1.1',
]

version = __version__

setup(
    python_requires='>=3.6',
    name='xmsmesher',
    version=version,
    packages=['xms.mesher', 'xms.mesher.meshing', 'xms.mesher.api'],
    include_package_data=True,
    license='BSD 2-Clause License',
    description='',
    author='Aquaveo',
    install_requires=requires,
    package_data={'': ['*.pyd', '*.so']},
)

