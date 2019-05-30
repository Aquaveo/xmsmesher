from .._xmsmesher.meshing import PolyInput as PInput
from xms import interp


class PolyInput(object):
    """
    Polygon inputs for meshing.

    Args:
        outside_polygon: *see below*
        inside_polygons: *see below*
        bias: *see below*
        size_function: *see below*
        polygon_corners: *see below*
        elev_function: *see below*
        boundary_points_to_remove: *see below*
        constant_size_function: *see below*
        constant_size_bias: *see below*
        polygon_id: *see below*
        seed_points: *see below*
        relaxation_method: *see below*
        remove_internal_four_triangle_points: *see below*
    """

    def __init__(self, outside_polygon=None, inside_polygons=None, bias=1.0, size_function=None,
                 polygon_corners=None, elev_function=None, boundary_points_to_remove=None,
                 constant_size_function=None, constant_size_bias=None, polygon_id=None, seed_points=None,
                 relaxation_method=None, remove_internal_four_triangle_points=None, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
        else:
            if outside_polygon is None:
                outside_polygon = []
            if inside_polygons is None:
                inside_polygons = []
            if polygon_corners is None:
                polygon_corners = []
            if size_function is not None:
                size_function = size_function._instance
            if elev_function is not None:
                elev_function = elev_function._instance
            self._instance = PInput(outside_polygon, inside_polygons, bias, size_function, polygon_corners,
                                    elev_function)

            if boundary_points_to_remove is not None:
                self.boundary_points_to_remove = boundary_points_to_remove
            if constant_size_function is not None:
                self.constant_size_function = constant_size_function
            if constant_size_bias is not None:
                self.constant_size_bias = constant_size_bias
            if polygon_id is not None:
                self.polygon_id = polygon_id
            if seed_points is not None:
                self.seed_points = seed_points
            if relaxation_method is not None:
                self.relaxation_method = relaxation_method
            if remove_internal_four_triangle_points is not None:
                self.remove_internal_four_triangle_points = remove_internal_four_triangle_points

    @property
    def outside_polygon(self):
        """List of points defining the outside polygon."""
        return self._instance.outPoly

    @outside_polygon.setter
    def outside_polygon(self, value):
        self._instance.outPoly = value

    @property
    def inside_polygons(self):
        """
        A list of polygons representing holes in the PolyInput.

        The polygons should be in clockwise order and the first point
        must not equal the last point.
        """
        return self._instance.insidePolys

    @inside_polygons.setter
    def inside_polygons(self, value):
        self._instance.insidePolys = value

    @property
    def polygon_corners(self):
        """
        Corner nodes for creating meshes using the patch algorithm.

        There can be 3 polygon_corners per outer_polygon not 4. The outer_polygon point at
        index 0 is assumed to be a corner.
        """
        return self._instance.polyCorners

    @polygon_corners.setter
    def polygon_corners(self, value):
        self._instance.polyCorners = value

    @property
    def boundary_points_to_remove(self):
        """Outer boundary locations to remove after the paving process."""
        return self._instance.boundPtsToRemove

    @boundary_points_to_remove.setter
    def boundary_points_to_remove(self, value):
        self._instance.boundPtsToRemove = value

    @property
    def bias(self):
        """Factor for transitioning between areas of high refinement to less refinement."""
        return self._instance.bias

    @bias.setter
    def bias(self, value):
        self._instance.bias = value

    @property
    def size_function(self):
        """Size function for scalar paving."""
        size_func = self._instance.sizeFunction
        if size_func is None:
            return None
        elif self._size_function_type == xmsinterp._xmsinterp.interpolate.InterpLinear:
            return xmsinterp.interpolate.InterpLinear(instance=size_func)
        elif self._size_function_type == xmsinterp._xmsinterp.interpolate.InterpIdw:
            return xmsinterp.interpolate.InterpIdw(instance=size_func)
        elif self._size_function_type == xmsinterp._xmsinterp.interpolate.InterpAnisotropic:
            return xmsinterp.interpolate.InterpAnisotropic(instance=size_func)
        else:
            raise RuntimeError("Unknown interp type: {}".format(type(size_func)))

    @size_function.setter
    def size_function(self, value):
        self._instance.sizeFunction = value._instance

    @property
    def elevation_function(self):
        """Elevation function for interpolating z coordinate of mesh points."""
        elev_function = self._instance.elevFunction
        if elev_function is None:
            return None
        elif self._elev_function_type == xmsinterp._xmsinterp.interpolate.InterpLinear:
            return xmsinterp.interpolate.InterpLinear(instance=elev_function)
        elif self._elev_function_type == xmsinterp._xmsinterp.interpolate.InterpIdw:
            return xmsinterp.interpolate.InterpIdw(instance=elev_function)
        elif self._elev_function_type == xmsinterp._xmsinterp.interpolate.InterpAnisotropic:
            return xmsinterp.interpolate.InterpAnisotropic(instance=elev_function)
        else:
            raise RuntimeError("Unknown interp type: {}".format(type(elev_function)))

    @elevation_function.setter
    def elevation_function(self, value):
        self._instance.elevFunction = value._instance

    @property
    def constant_size_function(self):
        """Constant to be used for size function."""
        return self._instance.constSizeFunction

    @constant_size_function.setter
    def constant_size_function(self, value):
        self._instance.constSizeFunction = value

    @property
    def constant_size_bias(self):
        """Constant to be used for size function."""
        return self._instance.constSizeBias

    @constant_size_bias.setter
    def constant_size_bias(self, value):
        self._instance.constSizeBias = value

    @property
    def polygon_id(self):
        """Transition factor for constant size function."""
        return self._instance.polyId

    @polygon_id.setter
    def polygon_id(self, value):
        self._instance.polyId = value

    @property
    def seed_points(self):
        """
        A list of seed points. If the user has some methodology for creating points inside the polygon
        then those points can be specified here. If these points are specified then the paving is not
        performed. These points will not be used if the meshing option is patch.
        """
        return self._instance.seedPoints

    @seed_points.setter
    def seed_points(self, value):
        self._instance.seedPoints = value

    @property
    def remove_internal_four_triangle_points(self):
        """Remove internal points that are only connected to 4 cells."""
        return self._instance.removeInternalFourTrianglePts

    @remove_internal_four_triangle_points.setter
    def remove_internal_four_triangle_points(self, value):
        self._instance.removeInternalFourTrianglePts = value

    @property
    def relaxation_method(self):
        """
        Relaxation method. The default relaxation method is an area relax. Set the value to “spring_relaxation”.
        See MeRelaxer.cpp for details on spring relaxation.
        """
        return self._instance.relaxationMethod

    @relaxation_method.setter
    def relaxation_method(self, value):
        self._instance.relaxationMethod = value
