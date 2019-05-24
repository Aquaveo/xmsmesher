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
#include <xmscore/python/misc/PyUtils.h>
#include <xmsinterp/interpolate/InterpBase.h>
#include <xmsmesher/meshing/MePolyRedistributePts.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initMePolyRedistributePts(py::module &m) {

    py::class_<xms::MePolyRedistributePts, boost::shared_ptr<xms::MePolyRedistributePts>> polyRedistribute(m, "PolyRedistributePts");

    polyRedistribute.def(py::init(&xms::MePolyRedistributePts::New));
    // -------------------------------------------------------------------------
    // function: SetSizeFunc
    // -------------------------------------------------------------------------
    polyRedistribute.def("SetSizeFunc", [](xms::MePolyRedistributePts &self, boost::shared_ptr<xms::InterpBase>  interp) {
      self.SetSizeFunc(interp);
    },py::arg("interp"));
    // -------------------------------------------------------------------------
    // function: SetSizeFuncFromPoly
    // -------------------------------------------------------------------------
    polyRedistribute.def("SetSizeFuncFromPoly", [](xms::MePolyRedistributePts &self, py::iterable out_poly, 
            py::iterable inside_polys, double size_bias) {
      xms::VecPt3d vec_out_poly = *xms::VecPt3dFromPyIter(out_poly);
      xms::VecPt3d2d vec_inside_polys = *xms::VecPt3d2dFromPyIter(inside_polys);
      self.SetSizeFuncFromPoly(vec_out_poly, vec_inside_polys, size_bias);
    }, py::arg("out_poly"), py::arg("inside_polys"),py::arg("size_bias"));
    // -------------------------------------------------------------------------
    // function: SetConstantSizeFunc
    // -------------------------------------------------------------------------
    polyRedistribute.def("SetConstantSizeFunc", &xms::MePolyRedistributePts::SetConstantSizeFunc, py::arg("size"));
    // -------------------------------------------------------------------------
    // function: SetConstantSizeBias
    // -------------------------------------------------------------------------
    polyRedistribute.def("SetConstantSizeBias", &xms::MePolyRedistributePts::SetConstantSizeBias, py::arg("size_bias"));
    // -------------------------------------------------------------------------
    // function: SetUseCurvatureRedistribution
    // -------------------------------------------------------------------------
    polyRedistribute.def("SetUseCurvatureRedistribution", &xms::MePolyRedistributePts::SetUseCurvatureRedistribution, 
      py::arg("feature_size"), py::arg("mean_spacing"),py::arg("minimum_curvature"),py::arg("smooth"));
    // -------------------------------------------------------------------------
    // function: Redistribute
    // -------------------------------------------------------------------------
    polyRedistribute.def("Redistribute", [](xms::MePolyRedistributePts &self,
                                py::iterable poly_line) -> py::iterable {
      xms::VecPt3d vec_poly_line = *xms::VecPt3dFromPyIter(poly_line);
      xms::VecPt3d vec_pts(self.Redistribute(vec_poly_line));
      return xms::PyIterFromVecPt3d(self.Redistribute(vec_poly_line));
    },py::arg("poly_line"));
}