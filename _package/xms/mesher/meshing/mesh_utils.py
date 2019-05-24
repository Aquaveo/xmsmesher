from .._xmsmesher.meshing import mesh_utils
from .multi_poly_mesher_io import MultiPolyMesherIo


def size_function_from_depth(depths, min_size, max_size):
    return mesh_utils.SizeFunctionFromDepth(depths, min_size, max_size)


def smooth_size_function(tin, sizes, size_ratio, min_size, anchor_to, points_flag):
    anchor_types = {'min': 0, 'max': 1}
    if anchor_to not in anchor_types.keys():
        raise ValueError("anchor_to must be one of 'min' or 'max', not {}".format(anchor_to))
    return mesh_utils.SmoothSizeFunction(tin._instance, sizes, size_ratio, min_size, anchor_types[anchor_to], points_flag)


def smooth_elev_by_slope(tin, elevations, max_slope, anchor_to, points_flag):
    anchor_types = {'min': 0, 'max': 1}
    if anchor_to not in anchor_types.keys():
        raise ValueError("anchor_to must be one of 'min' or 'max', not {}".format(anchor_to))
    return mesh_utils.SmoothElevBySlope(tin._instance, elevations, max_slope, anchor_types[anchor_to], points_flag)


def generate_mesh(mesh_io):
    return mesh_utils.generate_mesh(mesh_io._instance)


def generate_2dm(mesh_io, file_name, precision=15):
    return mesh_utils.generate_2dm(mesh_io._instance, file_name, precision)


def check_mesh_input_topology(mesh_io):
    return mesh_utils.check_mesh_input_topology(mesh_io._instance)


def redistribute_poly_line(polyline, size):
    return mesh_utils.redistribute_poly_line(polyline, size)
