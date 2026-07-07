//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <boost/shared_ptr.hpp>
#include <xmscore/misc/boost_defines.h>
#include <xmscore/python/misc/PyUtils.h>
#include <xmsinterp/interpolate/InterpBase.h>
#include <xmsmesher/meshing/InterpRasterSizeFunction.h>
#include <xmsmesher/python/meshing/meshing_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initInterpRasterSizeFunction(py::module& m)
{
  // xms::InterpBase's py::class_ is registered in the separate _xmsinterp extension module.
  // Force it to load first so pybind11 has the base type registered before we declare a
  // derived class here, regardless of which xms.mesher submodule Python imports first.
  py::module::import("xms.interp.interpolate");

  py::class_<xms::InterpRasterSizeFunction, xms::InterpBase,
    boost::shared_ptr<xms::InterpRasterSizeFunction>>
    iRsf(m, "InterpRasterSizeFunction");

  iRsf.def(
    py::init([](double x0, double y0, double dx, double dy,
                int nx, int ny, py::iterable values, float nodata) {
      boost::shared_ptr<xms::VecFlt> vec_values = xms::VecFltFromPyIter(values);
      return xms::InterpRasterSizeFunction::New(x0, y0, dx, dy, nx, ny, *vec_values, nodata);
    }),
    py::arg("x0"), py::arg("y0"),
    py::arg("dx"), py::arg("dy"),
    py::arg("nx"), py::arg("ny"),
    py::arg("values"),
    py::arg("nodata") = -1.0e38f);

  iRsf.def(
    "InterpToPt",
    [](xms::InterpRasterSizeFunction& self, py::tuple pt) -> float {
      xms::Pt3d point = xms::Pt3dFromPyIter(pt);
      return self.InterpToPt(point);
    },
    py::arg("pt"));

  iRsf.def(
    "InterpToPts",
    [](xms::InterpRasterSizeFunction& self, py::iterable pts) -> py::iterable {
      boost::shared_ptr<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
      boost::shared_ptr<xms::VecFlt> vec_scalars(new xms::VecFlt());
      self.InterpToPts(*vec_pts, *vec_scalars);
      return xms::PyIterFromVecFlt(*vec_scalars, py::isinstance<py::array>(pts));
    },
    py::arg("pts"));

  iRsf.def("SetTrunc", &xms::InterpRasterSizeFunction::SetTrunc,
    py::arg("smax"), py::arg("smin"));

  iRsf.def_property_readonly("GetPts", [](xms::InterpRasterSizeFunction& self) -> py::iterable {
    BSHP<xms::VecPt3d> pts = self.GetPts();
    return xms::PyIterFromVecPt3d(*pts);
  });
  iRsf.def_property_readonly("GetTris", [](xms::InterpRasterSizeFunction& self) -> py::iterable {
    BSHP<xms::VecInt> tris = self.GetTris();
    return xms::PyIterFromVecInt(*tris);
  });
  iRsf.def("GetScalars", &xms::InterpRasterSizeFunction::GetScalars);
  iRsf.def("GetPtActivity", &xms::InterpRasterSizeFunction::GetPtActivity);
  iRsf.def("GetTriActivity", &xms::InterpRasterSizeFunction::GetTriActivity);

  iRsf.def_property_readonly("GetTruncateInterpolatedValues",
    &xms::InterpRasterSizeFunction::GetTruncateInterpolatedValues);
  iRsf.def_property_readonly("GetTruncMin", &xms::InterpRasterSizeFunction::GetTruncMin);
  iRsf.def_property_readonly("GetTruncMax", &xms::InterpRasterSizeFunction::GetTruncMax);

  iRsf.def("__str__", &xms::InterpRasterSizeFunction::ToString);
}
