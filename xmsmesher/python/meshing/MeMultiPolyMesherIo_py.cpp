//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <iostream>
#include <sstream>
#include <pybind11/numpy.h>
#include <boost/shared_ptr.hpp>

#include <xmscore/misc/boost_defines.h>
#include <xmscore/python/misc/PyUtils.h>

#include <xmsinterp/interpolate/InterpLinear.h>

#include <xmsmesher/python/meshing/meshing_py.h>
#include <xmsmesher/meshing/MeMultiPolyMesherIo.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, BSHP<T>);

void initMeMultiPolyMesherIo(py::module &m) {

    py::class_<xms::MeMultiPolyMesherIo, BSHP<xms::MeMultiPolyMesherIo>> polyMesherIo(m, "MultiPolyMesherIo");

    polyMesherIo.def(py::init<>());
    // ---------------------------------------------------------------------------
    // PROPERTY: checkTopology
    // ---------------------------------------------------------------------------
    polyMesherIo.def_readwrite("checkTopology", &xms::MeMultiPolyMesherIo::m_checkTopology);
    // ---------------------------------------------------------------------------
    // PROPERTY: returnCellPolygons
    // ---------------------------------------------------------------------------
    polyMesherIo.def_readwrite("returnCellPolygons", &xms::MeMultiPolyMesherIo::m_returnCellPolygons);
    // ---------------------------------------------------------------------------
    // PROPERTY: points
    // ---------------------------------------------------------------------------
    polyMesherIo.def_property("points",
        [](xms::MeMultiPolyMesherIo &self) -> py::iterable {
            return xms::PyIterFromVecPt3d(self.m_points);
        },
        [](xms::MeMultiPolyMesherIo &self, py::iterable outside_polygon) {
            self.m_points = *xms::VecPt3dFromPyIter(outside_polygon);
        });
    // ---------------------------------------------------------------------------
    // PROPERTY: cells
    // ---------------------------------------------------------------------------
    polyMesherIo.def_property("cells",
            [](xms::MeMultiPolyMesherIo &self) -> py::iterable {
                return xms::PyIterFromVecInt(self.m_cells);
            },
            [](xms::MeMultiPolyMesherIo &self, py::iterable cells) {
                 self.m_cells = *xms::VecIntFromPyIter(cells);
            });
    // ---------------------------------------------------------------------------
    // PROPERTY: cellPolygons
    // ---------------------------------------------------------------------------
    polyMesherIo.def_property("cellPolygons",
            [](xms::MeMultiPolyMesherIo &self) -> py::iterable {
                return xms::PyIterFromVecInt(self.m_cellPolygons);
            },
            [](xms::MeMultiPolyMesherIo &self, py::iterable cell_polygons) {
                self.m_cellPolygons = *xms::VecIntFromPyIter(cell_polygons);
            });
    // ---------------------------------------------------------------------------
    // PROPERTY: polys
    // ---------------------------------------------------------------------------
    polyMesherIo.def_property("polys",
            [](xms::MeMultiPolyMesherIo &self) -> py::iterable {
                py::tuple ret_tuple(self.m_polys.size());
                for (int i = 0; i < self.m_polys.size(); i++) {
                    ret_tuple[i] = self.m_polys[i];
                }
                return ret_tuple;
            },
            [](xms::MeMultiPolyMesherIo &self, py::iterable polys) {
                 std::vector<xms::MePolyInput> &vecPolys = self.m_polys;
                 vecPolys.clear();
                 vecPolys.reserve(py::len(polys));
                 for (auto item : polys) {
                    vecPolys.push_back(item.cast<xms::MePolyInput>());
                 }
            });
    // ---------------------------------------------------------------------------
    // PROPERTY: refPts
    // ---------------------------------------------------------------------------
    polyMesherIo.def_property("refPts",
            [](xms::MeMultiPolyMesherIo &self) -> py::iterable {
                py::tuple ret_tuple(self.m_refPts.size());
                for (int i = 0; i < self.m_refPts.size(); i++) {
                    ret_tuple[i] = self.m_refPts[i];
                }
                return ret_tuple;
            },
            [](xms::MeMultiPolyMesherIo &self, py::iterable refine_points) {
                 std::vector<xms::MeRefinePoint> &vecRefinePoints = self.m_refPts;
                 vecRefinePoints.clear();
                 vecRefinePoints.reserve(py::len(refine_points));
                 for (auto item : refine_points) {
                    vecRefinePoints.push_back(item.cast<xms::MeRefinePoint>());
                 }
            });
}

void initMePolyInput(py::module &m) {

    py::class_<xms::MePolyInput, BSHP<xms::MePolyInput>> polyInput(m, "PolyInput");

    polyInput.def(py::init<>([](py::iterable outside_polygon, py::iterable inside_polygons, double bias,
                            py::object size_function, py::iterable patch_polygon_corners, py::object elev_function) {
            xms::VecPt3d vec_outside_polygon = *xms::VecPt3dFromPyIter(outside_polygon);
            xms::VecPt3d2d vec_inside_polygons = *xms::VecPt3d2dFromPyIter(inside_polygons);
            xms::VecInt vec_poly_corners = *xms::VecIntFromPyIter(patch_polygon_corners);
            BSHP<xms::InterpBase> c_size_function;
            BSHP<xms::InterpBase> c_elev_function;
            if (!size_function.is_none())
            {
              c_size_function = size_function.cast<BSHP<xms::InterpBase>>();
            }
            if (!elev_function.is_none())
            {
              c_elev_function = elev_function.cast<BSHP<xms::InterpBase>>();
            }
            BSHP<xms::MePolyInput> rval(new xms::MePolyInput(vec_outside_polygon, vec_inside_polygons, bias, c_size_function,
                                                         vec_poly_corners, c_elev_function));
            return rval;
        }), py::arg("outside_polygon"), py::arg("inside_polygons"), py::arg("bias"),
            py::arg("size_function"), py::arg("patch_polygon_corners"),
            py::arg("elev_function"));
    // ---------------------------------------------------------------------------
    // function: outside_polygon
    // ---------------------------------------------------------------------------
    polyInput.def_property("outPoly",
            [](xms::MePolyInput &self) -> py::iterable {
                return xms::PyIterFromVecPt3d(self.m_outPoly);
            },
            [](xms::MePolyInput &self, py::iterable outside_polygon) {
                 self.m_outPoly = * xms::VecPt3dFromPyIter(outside_polygon);
            });
    // ---------------------------------------------------------------------------
    // function: insidePolys
    // ---------------------------------------------------------------------------
    polyInput.def_property("insidePolys",
            [](xms::MePolyInput &self) -> py::iterable {
                return xms::PyIterFromVecPt3d2d(self.m_insidePolys);
            },
            [](xms::MePolyInput &self, py::iterable inside_polygons) {
                self.m_insidePolys = *xms::VecPt3d2dFromPyIter(inside_polygons);
            });
    // -------------------------------------------------------------------------
    // function: polyCorners
    // -------------------------------------------------------------------------
    polyInput.def_property("polyCorners",
            [](xms::MePolyInput &self) -> py::iterable {
                return xms::PyIterFromVecInt(self.m_polyCorners);
            },
            [](xms::MePolyInput &self, py::iterable patch_polygon_corners) {
                 self.m_polyCorners = *xms::VecIntFromPyIter(patch_polygon_corners);
            });
    // -------------------------------------------------------------------------
    // function: boundPtsToRemove
    // -------------------------------------------------------------------------
    polyInput.def_property("boundPtsToRemove",
            [](xms::MePolyInput &self) -> py::iterable {
                return xms::PyIterFromVecPt3d(self.m_boundPtsToRemove);
            },
            [](xms::MePolyInput &self, py::iterable outside_polygon) {
                 self.m_boundPtsToRemove = *xms::VecPt3dFromPyIter(outside_polygon);
            });
    // -------------------------------------------------------------------------
    // function: bias
    // -------------------------------------------------------------------------
    polyInput.def_readwrite("bias", &xms::MePolyInput::m_bias);
    // -------------------------------------------------------------------------
    // function: sizeFunction
    // -------------------------------------------------------------------------
    polyInput.def_readwrite("sizeFunction", &xms::MePolyInput::m_sizeFunction);
    // -------------------------------------------------------------------------
    // function: elevFunction
    // -------------------------------------------------------------------------
    polyInput.def_readwrite("elevFunction", &xms::MePolyInput::m_elevFunction);
    // -------------------------------------------------------------------------
    // function: constSizeFunction
    // -------------------------------------------------------------------------
    polyInput.def_readwrite("constSizeFunction", &xms::MePolyInput::m_constSizeFunction);
    // -------------------------------------------------------------------------
    // function: constSizeBias
    // -------------------------------------------------------------------------
    polyInput.def_readwrite("constSizeBias", &xms::MePolyInput::m_constSizeBias);
    // -------------------------------------------------------------------------
    // function: removeInternalFourTrianglePts
    // -------------------------------------------------------------------------
    polyInput.def_readwrite("removeInternalFourTrianglePts", &xms::MePolyInput::m_removeInternalFourTrianglePts);
    // -------------------------------------------------------------------------
    // function: poly_id
    // -------------------------------------------------------------------------
    polyInput.def_readwrite("polyId", &xms::MePolyInput::m_polyId);
    // -------------------------------------------------------------------------
    // function: seedPoints
    // -------------------------------------------------------------------------
    polyInput.def_property("seedPoints",
            [](xms::MePolyInput &self) -> py::iterable {
                return xms::PyIterFromVecPt3d(self.m_seedPoints);
            },
            [](xms::MePolyInput &self, py::iterable seed_points) {
                 self.m_seedPoints = *xms::VecPt3dFromPyIter(seed_points);
            });
    // -------------------------------------------------------------------------
    // function: poly_id
    // -------------------------------------------------------------------------
    polyInput.def_readwrite("relaxSeedPoints", &xms::MePolyInput::m_relaxSeedPoints);
    // ---------------------------------------------------------------------------
    // property: relaxationMethod
    // ---------------------------------------------------------------------------
    polyInput.def_readwrite("relaxationMethod", &xms::MePolyInput::m_relaxationMethod);
}

void initMeRefinePoint(py::module &m) {

    py::class_<xms::MeRefinePoint, BSHP<xms::MeRefinePoint>> refinePoint(m, "RefinePoint");

    refinePoint.def(py::init<>([](py::tuple point, double size, bool create_mesh_point) {
            xms::Pt3d _point = xms::Pt3dFromPyIter(point);
            return new xms::MeRefinePoint(_point, size, create_mesh_point);
        }), py::arg("point"), py::arg("size"), py::arg("create_mesh_point"));
    // -------------------------------------------------------------------------
    // function: pt
    // -------------------------------------------------------------------------
    refinePoint.def_property("pt",
          [](xms::MeRefinePoint &self) -> py::tuple {
            return py::make_tuple(self.m_pt.x, self.m_pt.y, self.m_pt.z);
          },
          [](xms::MeRefinePoint &self, py::tuple pt) {
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            self.m_pt = point;
          });
    // -------------------------------------------------------------------------
    // function: size
    // -------------------------------------------------------------------------
    refinePoint.def_readwrite("size", &xms::MeRefinePoint::m_size);
    // -------------------------------------------------------------------------
    // function: createMeshPoint
    // -------------------------------------------------------------------------
    refinePoint.def_readwrite("createMeshPoint", &xms::MeRefinePoint::m_createMeshPoint);
}
