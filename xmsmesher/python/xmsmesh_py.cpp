//------------------------------------------------------------------------------
/// \file
/// \brief root module for xmsmesher Python bindings.
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <xmsmesher/python/meshing/meshing_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
#ifndef XMS_VERSION
  #define XMS_VERSION "99.99.99";
#endif

//------ Primary Module --------------------------------------------------------
PYBIND11_MODULE(_xmsmesher, m) {
    m.doc() = "Python bindings for xmsmesher"; // optional module docstring
    m.attr("__version__") = XMS_VERSION;

    // Interpolate module
    py::module modMeshing = m.def_submodule("meshing");
    initMeshing(modMeshing);
}