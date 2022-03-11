"""
conanfile.py for the xmsstamper project.
"""
from xmsconan.xms_conan_file import XmsConanFile


class XmsstamperConan(XmsConanFile):
    """
    XmsstamperConan class used for defining the conan info.
    """
    name = 'xmsstamper'
    url = 'https://github.com/Aquaveo/xmsstamper'
    description = 'Stamer library for XMS products'
    xms_dependencies = [
        'xmscore/[>=6.0.1 <7.0.0]@aquaveo/stable',
        'xmsgrid/[>=7.0.1 <8.0.0]@aquaveo/stable',
        'xmsinterp/[>=6.0.1 <7.0.0]@aquaveo/stable',
    ]
    extra_export_sources = [
        'test_files'
    ]


LIBRARY_NAME = XmsstamperConan.name
