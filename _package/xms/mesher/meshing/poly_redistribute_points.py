from .._xmsmesher.meshing import PolyRedistributePts as RedistPoints


class PolyRedistributePoints(object):

    def __init__(self, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
        else:
            self._instance = RedistPoints()

    def set_size_function_from_polygon(self, outside_polygon, inside_polygons, size_bias):
        self._instance.SetSizeFuncFromPoly(outside_polygon, inside_polygons, size_bias)

    def set_size_func(self, size_function):
        self._instance.SetSizeFunc(size_function._instance)

    def set_constant_size_function(self, size):
        self._instance.SetConstantSizeFunc(size)

    def set_constant_size_bias(self, size_bias):
        self._instance.SetConstantSizeBias(size_bias)

    def set_use_curvature_redistribution(self, feature_size, mean_spacing, minimum_curvature, smooth):
        self._instance.SetUseCurvatureRedistribution(feature_size, mean_spacing, minimum_curvature, smooth)

    def redistribute(self, polyline):
        return self._instance.Redistribute(polyline)
