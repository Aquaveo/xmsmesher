"""
conanfile.py for the xmsmesher project.
"""
from xmsconan.xms_conan2_file import XmsConan2File


class XmsmesherConanFile(XmsConan2File):
    """
    XmsmesherConanFile class used for defining the conan info.
    """
    name = "xmsmesher"
    url = "https://github.com/Aquaveo/xmsmesher"
    description = "Meshing library for XMS products"
    xms_dependencies = [
        "xmscore/6.2.5-6-g72e83110",
        "xmsgrid/5.5.3-53-g35e7e2f",
        "xmsinterp/6.1.1-12-g1d45191a",
    ]
    extra_export_sources = ['test_files']

LIBRARY_NAME = XmsmesherConanFile.name
