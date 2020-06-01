from .._xmsmesher.meshing import mesh_utils
from .multi_poly_mesher_io import MultiPolyMesherIo

__all__ = ['size_function_from_depth', 'smooth_size_function', 'smooth_elev_by_slope', 'generate_mesh', 'generate_2dm',
           'check_mesh_input_topology', 'redistribute_poly_line']


def size_function_from_depth(depths, min_size, max_size):
    """
    Creates a size at each point based on the depth at the point and the min and max sizes the
    equation is min_depth + ( (depth - min_depth) / (max_depth - min_depth) ) * (max_size - min_size).
    This is often useful for coastal numerical model simulations.

    Args:
        depths (iterable): The measured depths at point locations.
        min_size (float): The minimum element edge size.
        max_size (float): The maximum element edge size.

    Returns:
        Array of sizes based on depth.
    """
    return mesh_utils.SizeFunctionFromDepth(depths, min_size, max_size)


def size_function_from_edge_lengths(ugrid):
    """
    Creates a size at each point based on the average length of the connected edges to the point.

    Args:
        ugrid (UGrid): The unstructructure grid

    Returns:
        Array of sizes based on depth.
    """
    return mesh_utils.SizeFunctionFromEdgeLengths(ugrid._instance)


def smooth_size_function(tin, sizes, size_ratio, min_size, anchor_to, points_flag):
    """
    Smooths a size function. Ensures that the size function transitions over a sufficient distance so that the area
    change of adjacent elements meets the size ratio passed in.

    Args:
        tin (tin): Points and triangles defining the connectivity of the size function.
        sizes (iterable): Array of the current sizes.
        size_ratio (float): Allowable size difference between adjacent elements.
        min_size (float): Minimum specified element size.
        anchor_to (str): Option to anchor to the minimum or maximum size ('min' or 'max')
        points_flag(iterable): Flag to indicate if the value at the point should be adjusted (a value of true will skip
                               the point). Leave the bitset empty to process all points.

    Returns:
        Array of smoothed sizes.
    """
    anchor_types = {'min': 0, 'max': 1}
    if anchor_to not in anchor_types.keys():
        raise ValueError("anchor_to must be one of 'min' or 'max', not {}".format(anchor_to))
    return mesh_utils.SmoothSizeFunction(tin._instance, sizes, size_ratio, min_size, anchor_types[anchor_to], points_flag)


def smooth_size_function_ugrid(ugrid, sizes, size_ratio, min_size, anchor_to, points_flag):
    """
    Smooths a size function. Ensures that the size function transitions over a sufficient distance so that the area
    change of adjacent elements meets the size ratio passed in.

    Args:
        ugrid (UGrid): Unstructured grid defining the connectivity of the size function.
        sizes (iterable): Array of the current sizes.
        size_ratio (float): Allowable size difference between adjacent elements.
        min_size (float): Minimum specified element size.
        anchor_to (str): Option to anchor to the minimum or maximum size ('min' or 'max')
        points_flag(iterable): Flag to indicate if the value at the point should be adjusted (a value of true will skip
                               the point). Leave the bitset empty to process all points.

    Returns:
        Array of smoothed sizes.
    """
    anchor_types = {'min': 0, 'max': 1}
    if anchor_to not in anchor_types.keys():
        raise ValueError("anchor_to must be one of 'min' or 'max', not {}".format(anchor_to))
    return mesh_utils.SmoothSizeFunctionUGrid(ugrid._instance, sizes, size_ratio, min_size, anchor_types[anchor_to], points_flag)


def smooth_elev_by_slope(tin, elevations, max_slope, anchor_to, points_flag):
    """
    Smooths a elevations based on max specified slope (max_slope)
    preserving either the min or max based on anchor_type.

    Args:
        tin (tin): Points and triangles defining the connectivity of the elevations.
        elevations (iterable): Array of the current elevations.
        max_slope (float): Maximum allowable slope.
        anchor_to (bool): Indicates if you are anchoring to the max slope.
        points_flag (iterable): Flag to indicate if the value at the point should be adjusted (a value of true will
                                skip the point). Leave the bitset empty to process all points.

    Returns:
        Array of smoothed elevations.
    """
    anchor_types = {'min': 0, 'max': 1}
    if anchor_to not in anchor_types.keys():
        raise ValueError("anchor_to must be one of 'min' or 'max', not {}".format(anchor_to))
    return mesh_utils.SmoothElevBySlope(tin._instance, elevations, max_slope, anchor_types[anchor_to], points_flag)


def smooth_elev_by_slope_ugrid(ugrid, elevations, max_slope, anchor_to, points_flag):
    """
    Smooths a elevations based on max specified slope (max_slope)
    preserving either the min or max based on anchor_type.

    Args:
        ugrid (UGrid): Unstructured grid defining the connectivity of the elevations.
        elevations (iterable): Array of the current elevations.
        max_slope (float): Maximum allowable slope.
        anchor_to (bool): Indicates if you are anchoring to the max slope.
        points_flag (iterable): Flag to indicate if the value at the point should be adjusted (a value of true will
                                skip the point). Leave the bitset empty to process all points.

    Returns:
        Array of smoothed elevations.
    """
    anchor_types = {'min': 0, 'max': 1}
    if anchor_to not in anchor_types.keys():
        raise ValueError("anchor_to must be one of 'min' or 'max', not {}".format(anchor_to))
    return mesh_utils.SmoothElevBySlopeUGrid(ugrid._instance, elevations, max_slope, anchor_types[anchor_to], points_flag)


def generate_mesh(mesh_io):
    """
    Creates a mesh from the input polygons.

    Args:
        mesh_io (MultiPolyMesherIo): Input polygons and options for generating a mesh.

    Returns:
        true if the mesh was generated successfully false otherwise, and a string of messages.
    """
    return mesh_utils.generate_mesh(mesh_io._instance)


def generate_2dm(mesh_io, file_name, precision=15):
    """
    Creates a mesh from the input polygons and writes it to a 2dm file.

    Args:
        mesh_io (MultiPolyMesherIo): Input polygons and options for generating a mesh.
        file_name (str): The file name of the output 2dm file.
        precision (int, optional): The decimal point precision of the resulting mesh.

    Returns:
        true if the mesh was generated successfully false otherwise, and a string of messages.
    """
    return mesh_utils.generate_2dm(mesh_io._instance, file_name, precision)


def check_mesh_input_topology(mesh_io):
    """
    Checks if the input polygons intersect one another.

    Args:
        mesh_io (MultiPolyMesherIo): Input polygons and options for generating a mesh.

    Returns:
        true if mesh inputs are topologically correct, and a string of messages.
    """
    return mesh_utils.check_mesh_input_topology(mesh_io._instance)


def redistribute_poly_line(polyline, size):
    """
    Redistributes the points along a line to a constant spacing.

    Args:
        polyline (iterable): Input poly line locations.
        size (float): The desired spacing for point redistribution.

    Returns:
        redistributed poly line locations.
    """
    return mesh_utils.redistribute_poly_line(polyline, size)
