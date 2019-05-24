from .._xmsmesher.meshing import PolyInput as PInput
from xms import interp


class PolyInput(object):

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
        """"""
        return self._instance.outPoly

    @outside_polygon.setter
    def outside_polygon(self, value):
        self._instance.outPoly = value

    @property
    def inside_polygons(self):
        """"""
        return self._instance.insidePolys

    @inside_polygons.setter
    def inside_polygons(self, value):
        self._instance.insidePolys = value

    @property
    def polygon_corners(self):
        """"""
        return self._instance.polyCorners

    @polygon_corners.setter
    def polygon_corners(self, value):
        self._instance.polyCorners = value

    @property
    def boundary_points_to_remove(self):
        """"""
        return self._instance.boundPtsToRemove

    @boundary_points_to_remove.setter
    def boundary_points_to_remove(self, value):
        self._instance.boundPtsToRemove = value

    @property
    def bias(self):
        """"""
        return self._instance.bias

    @bias.setter
    def bias(self, value):
        self._instance.bias = value

    @property
    def size_function(self):
        """"""
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
        """"""
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
        """"""
        return self._instance.constSizeFunction

    @constant_size_function.setter
    def constant_size_function(self, value):
        self._instance.constSizeFunction = value

    @property
    def constant_size_bias(self):
        """"""
        return self._instance.constSizeBias

    @constant_size_bias.setter
    def constant_size_bias(self, value):
        self._instance.constSizeBias = value

    @property
    def polygon_id(self):
        """"""
        return self._instance.polyId

    @polygon_id.setter
    def polygon_id(self, value):
        self._instance.polyId = value

    @property
    def seed_points(self):
        """"""
        return self._instance.seedPoints

    @seed_points.setter
    def seed_points(self, value):
        self._instance.seedPoints = value

    @property
    def remove_internal_four_triangle_points(self):
        """"""
        return self._instance.removeInternalFourTrianglePts

    @remove_internal_four_triangle_points.setter
    def remove_internal_four_triangle_points(self, value):
        self._instance.removeInternalFourTrianglePts = value

    @property
    def relaxation_method(self):
        """"""
        return self._instance.relaxationMethod

    @relaxation_method.setter
    def relaxation_method(self, value):
        self._instance.relaxationMethod = value
