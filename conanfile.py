"""
conanfile.py for the xmsmesher project.
"""
from xmsconan.xms_conan_file import XmsConanFile


class XmsmesherConan(XmsConanFile):
    """
    XmsstamperConan class used for defining the conan info.
    """
    name = 'xmsmesher'
    url = 'https://github.com/Aquaveo/xmsmesher'
    description = 'Mesher library for XMS products'
    xms_dependencies = [
        'xmscore/[>=6.0.1 <7.0.0]@aquaveo/stable',
        'xmsgrid/[>=7.0.1 <8.0.0]@aquaveo/stable',
        'xmsinterp/[>=6.0.1 <7.0.0]@aquaveo/stable',
    ]
    extra_export_sources = [
        'test_files'
    ]


LIBRARY_NAME = XmsmesherConan.name
