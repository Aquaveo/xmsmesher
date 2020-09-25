"""Class for redistributing the points of a polygon."""
from .._xmsmesher.meshing import PolyRedistributePts as RedistPoints


class PolyRedistributePoints(object):
    """Class for redistributing the points of a polygon."""
    def __init__(self, **kwargs):
        """Constructor.

        Args:
            **kwargs (dict): Generic keyword arguments
        """
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
        else:
            self._instance = RedistPoints()

    def set_size_func(self, size_function):
        """Sets the size function interpolator.

        Args:
            size_function (InterpBase): Size function interpolator class.
        """
        self._instance.SetSizeFunc(size_function._instance)

    def set_size_function_from_polygon(self, outside_polygon, inside_polygons, size_bias):
        """Creates an interpolator that uses the spacing on the input polygon as its scalar.

        Args:
            outside_polygon (iterable): The outside polygon
            inside_polygons (iterable): Inside polygons that are inside of a_outPoly
            size_bias (float): A factor used in transitioning the size
        """
        self._instance.SetSizeFuncFromPoly(outside_polygon, inside_polygons, size_bias)

    def set_constant_size_function(self, size):
        """Sets the size function to a constant value.

        Args:
            size (float): The element edge size.
        """
        self._instance.SetConstantSizeFunc(size)

    def set_constant_size_bias(self, size_bias):
        """Sets the bias for constant value size function.

        Args:
            size_bias (float): Transition rate for size function.
        """
        self._instance.SetConstantSizeBias(size_bias)

    def set_use_curvature_redistribution(self, feature_size, mean_spacing, minimum_curvature, smooth):
        """Specifies that curvature redistribution will be used.

        Args:
            feature_size (float): The size of the smallest feature in the polyline to be detected.
            mean_spacing (float): The mean spacing between the distributed points.
            minimum_curvature (float): The value of the curvature to be used instead of 0 in straight lines. It limits
                the maximum spacing between points. If not included, the default is 0.001.
            smooth (bool): Determines if the curvatures are to be averaged by a rolling 0.25-0.5-0.25 weighted rolling
                average.
        """
        self._instance.SetUseCurvatureRedistribution(feature_size, mean_spacing, minimum_curvature, smooth)

    def redistribute(self, polyline):
        """Redistributes points on closed loop polylines.

        The length of edges in the redistribution comes from a size function that is interpolated to the points
        that make up the polylines. By default this size function comes from the edge lengths in the original polygon.

        Args:
            polyline (iterable): List of (x, y, z) coordinates defining the polygon.

        Returns:
            Redistributed polyline.
        """
        return self._instance.Redistribute(polyline)
