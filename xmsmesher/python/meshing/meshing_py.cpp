//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <xmscore/python/misc/PyUtils.h>
#include <xmsmesher/meshing/MeMultiPolyMesherIo.h>
#include <xmsmesher/python/meshing/meshing_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------

void initMeshing(py::module &m) {
    initMeMeshUtils(m);
    initMePolyInput(m);
    initMeRefinePoint(m);
    initMeMultiPolyMesherIo(m);
    initMePolyRedistributePts(m);
}
