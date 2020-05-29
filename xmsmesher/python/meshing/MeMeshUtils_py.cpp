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
#include <xmscore/misc/DynBitset.h>
#include <xmscore/misc/XmError.h>
#include <xmscore/python/misc/PyUtils.h>
#include <xmsgrid/triangulate/TrTin.h>
#include <xmsgrid/ugrid/XmUGrid.h>
#include <xmsmesher/meshing/MeMeshUtils.h>
#include <xmsmesher/meshing/MeMultiPolyMesher.h>
#include <xmsmesher/meshing/MeMultiPolyMesherIo.h>
#include <xmsmesher/meshing/MeMultiPolyTo2dm.h>
#include <xmsmesher/meshing/MePolyRedistributePts.h>


//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initMeMeshUtils(py::module &m) {

    py::module modMeshUtils = m.def_submodule("mesh_utils");

  // ---------------------------------------------------------------------------
  // function: size_function_from_depth
  // ---------------------------------------------------------------------------
    modMeshUtils.def("SizeFunctionFromDepth", [](py::iterable depths, double min_size,
                                         double max_size) -> py::iterable {

        xms::VecDbl vec_depths, vec_size;
        vec_depths = *xms::VecDblFromPyIter(depths);
        xms::meSizeFunctionFromDepth(vec_depths, vec_size, min_size, max_size);
        return xms::PyIterFromVecDbl(vec_size);
    },py::arg("depths"),py::arg("min_size"),
    py::arg("max_size"));
	// ---------------------------------------------------------------------------
	// function: size_function_from_edge_lengths
	// ---------------------------------------------------------------------------
	  modMeshUtils.def("SizeFunctionFromEdgeLengths", [](py::object ugrid) -> py::iterable {
          xms::VecDbl vec_size;
          std::shared_ptr<xms::XmUGrid> ug;
          ug = ugrid.cast<std::shared_ptr<xms::XmUGrid>>();
		  xms::meSizeFunctionFromEdgeLengths(ug, vec_size);
		  return xms::PyIterFromVecDbl(vec_size);
	  }, py::arg("ugrid"));
  // ---------------------------------------------------------------------------
  // function: smooth_size_function
  // ---------------------------------------------------------------------------
    modMeshUtils.def("SmoothSizeFunction", [](boost::shared_ptr<xms::TrTin> tin, py::iterable sizes,
                                    double size_ratio, double min_size, int anchor_type,
                                    py::iterable pts_flag) -> py::iterable {
      xms::VecFlt vec_sizes = *xms::VecFltFromPyIter(sizes);
      xms::VecFlt vec_smooth_sizes;
      xms::DynBitset bitset =  xms::DynamicBitsetFromPyIter(pts_flag);

      xms::meSmoothSizeFunction(tin, vec_sizes, size_ratio, min_size, anchor_type, bitset, vec_smooth_sizes);

      return xms::PyIterFromVecFlt(vec_smooth_sizes);
    }, py::arg("tin"),py::arg("sizes"), py::arg("size_ratio"), py::arg("min_size"), 
       py::arg("anchor_type"), py::arg("pts_flag"));
  // ---------------------------------------------------------------------------
  // function: smooth_size_function_ugrid
  // ---------------------------------------------------------------------------
    modMeshUtils.def("SmoothSizeFunctionUGrid", [](py::object ugrid, py::iterable sizes,
                                    double size_ratio, double min_size, int anchor_type,
                                    py::iterable pts_flag) -> py::iterable {
      std::shared_ptr<xms::XmUGrid> ug;
      ug = ugrid.cast<std::shared_ptr<xms::XmUGrid>>();
      xms::VecFlt vec_sizes = *xms::VecFltFromPyIter(sizes);
      xms::VecFlt vec_smooth_sizes;
      xms::DynBitset bitset =  xms::DynamicBitsetFromPyIter(pts_flag);

      xms::meSmoothSizeFunction(ug, vec_sizes, size_ratio, min_size, anchor_type, bitset, vec_smooth_sizes);

      return xms::PyIterFromVecFlt(vec_smooth_sizes);
    }, py::arg("ugrid"),py::arg("sizes"), py::arg("size_ratio"), py::arg("min_size"),
       py::arg("anchor_type"), py::arg("pts_flag"));
  // ---------------------------------------------------------------------------
  // function: smooth_elev_by_slope
  // ---------------------------------------------------------------------------
    modMeshUtils.def("SmoothElevBySlope", [](boost::shared_ptr<xms::TrTin> tin, py::iterable elevations,
                                    double max_slope, int anchor_to_max,
                                    py::iterable pts_flag) -> py::iterable {
      xms::VecFlt vec_elevations = *xms::VecFltFromPyIter(elevations);
      xms::VecFlt vec_smooth_elevations;
      xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(pts_flag);
      xms::meSmoothElevBySlope(tin, vec_elevations, max_slope, anchor_to_max, bitset, vec_smooth_elevations);

      return xms::PyIterFromVecFlt(vec_smooth_elevations);
    }, py::arg("tin"),py::arg("elevations"), py::arg("max_slope"),py::arg("anchor_to_max"), py::arg("pts_flag"));
    // ---------------------------------------------------------------------------
    // function: smooth_elev_by_slope_ugrid
    // ---------------------------------------------------------------------------
    modMeshUtils.def("SmoothElevBySlopeUGrid", [](py::object ugrid, py::iterable elevations,
      double max_slope, int anchor_to_max,
      py::iterable pts_flag) -> py::iterable {
      std::shared_ptr<xms::XmUGrid> ug;  // This will not compile if the declaration and assignment are on the same line
      ug = ugrid.cast<std::shared_ptr<xms::XmUGrid>>();
      xms::VecFlt vec_elevations = *xms::VecFltFromPyIter(elevations);
      xms::VecFlt vec_smooth_elevations;
      xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(pts_flag);
      xms::meSmoothElevBySlope(ug, vec_elevations, max_slope, anchor_to_max, bitset, vec_smooth_elevations);

      return xms::PyIterFromVecFlt(vec_smooth_elevations);
    }, py::arg("ugrid"), py::arg("elevations"), py::arg("max_slope"), py::arg("anchor_to_max"), py::arg("pts_flag"));

  // ---------------------------------------------------------------------------
  // function: check_mesh_input_topology
  // ---------------------------------------------------------------------------
    modMeshUtils.def("check_mesh_input_topology",
     [](xms::MeMultiPolyMesherIo &mesh_io) -> py::iterable
     {
       BSHP<xms::MeMultiPolyMesher> multiPolyMesher = xms::MeMultiPolyMesher::New();
       std::string errors;
       multiPolyMesher->CheckForIntersections(mesh_io, errors);
       bool rval(errors.empty());
       return py::make_tuple(rval, errors);
     }, py::arg("mesh_io"));
  // ---------------------------------------------------------------------------
  // function: generate_mesh
  // ---------------------------------------------------------------------------
    modMeshUtils.def("generate_mesh",
     [](xms::MeMultiPolyMesherIo &mesh_io) -> py::iterable
     {
       BSHP<xms::MeMultiPolyMesher> multiPolyMesher = xms::MeMultiPolyMesher::New();
       bool rval = multiPolyMesher->MeshIt(mesh_io);
       std::string errors = xms::XmLog::Instance().GetAndClearStackStr();
       return py::make_tuple(rval, errors);
     }, py::arg("mesh_io"));
  // ---------------------------------------------------------------------------
  // function: generate_2dm
  // ---------------------------------------------------------------------------
    modMeshUtils.def("generate_2dm",
     [](xms::MeMultiPolyMesherIo &mesh_io,
        std::string file_name, int precision) -> py::tuple {
        BSHP<xms::MeMultiPolyTo2dm> mesher = xms::MeMultiPolyTo2dm::New();
        if (file_name.empty()) {
          throw py::value_error("file_name not specifed. Aborting mesh procedure.");
        }
        bool result = mesher->Generate2dm(mesh_io, file_name, precision);
        std::string errors = xms::XmLog::Instance().GetAndClearStackStr();
        return py::make_tuple(result, errors);
        },py::arg("mesh_io"),py::arg("file_name"),py::arg("precision"));

  // ---------------------------------------------------------------------------
  // function: redistribute_line
  // ---------------------------------------------------------------------------
    modMeshUtils.def("redistribute_poly_line",
     [](py::iterable poly_line, double size) -> py::iterable {
        BSHP<xms::MePolyRedistributePts> redist(xms::MePolyRedistributePts::New());
        redist->SetConstantSizeFunc(size);
        BSHP<xms::VecPt3d> vPolyLine = xms::VecPt3dFromPyIter(poly_line);
        xms::VecPt3d rval = redist->Redistribute(*vPolyLine);
        return xms::PyIterFromVecPt3d(rval);
        },py::arg("poly_line"),py::arg("size"));

}