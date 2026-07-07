"""Python wrapper for InterpRasterSizeFunction."""
from xms.interp.interpolate import Interpolator

from .._xmsmesher.meshing import InterpRasterSizeFunction as iRsf


class InterpRasterSizeFunction(Interpolator):
    """Mesh size function derived from a structured raster grid.

    Stores the raster as a compact grid description and evaluates sizes by
    direct grid lookup.  Points inside the raster use bilinear interpolation;
    points outside are extrapolated using the nearest boundary cell value.
    """

    def __init__(self, x0=None, y0=None, dx=None, dy=None, nx=None, ny=None, values=None,
                 nodata=-1.0e38, **kwargs):
        """Constructor.

        Args:
            x0 (float): X coordinate of the upper-left raster corner in display CRS.
            y0 (float): Y coordinate of the upper-left raster corner in display CRS.
            dx (float): Pixel width (positive).
            dy (float): Pixel height (negative for north-up rasters).
            nx (int): Number of columns.
            ny (int): Number of rows.
            values (iterable): Flat row-major array of size values; length must be nx * ny.
            nodata (float): Nodata sentinel value (informational only).
            **kwargs (dict): Generic keyword arguments
        """
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        self._instance = iRsf(x0, y0, dx, dy, nx, ny, values, nodata)
        super().__init__(**kwargs)

    def __eq__(self, other):
        """Equality operator.

        Args:
            other (InterpRasterSizeFunction): InterpRasterSizeFunction to compare

        Returns:
            bool: True if InterpRasterSizeFunctions are equal
        """
        other_instance = getattr(other, '_instance', None)
        if not other_instance or not isinstance(other_instance, iRsf):
            return False
        return other_instance == self._instance

    def __ne__(self, other):
        """Equality operator.

        Args:
            other (InterpRasterSizeFunction): InterpRasterSizeFunction to compare

        Returns:
            bool: True if InterpRasterSizeFunctions are not equal
        """
        return not self.__eq__(other)

    def __str__(self):
        """Return a string representation."""
        return self._instance.__str__()

    def __repr__(self):
        """Return a string representation."""
        return self._instance.__str__()

    def interpolate_to_point(self, point):
        """Interpolate the size at a single location.

        Args:
            point (tuple): (x, y, z) location to query.

        Returns:
            float: Interpolated size value.
        """
        return self._instance.InterpToPt(point)

    def interpolate_to_points(self, points):
        """Interpolate sizes at an array of locations.

        Args:
            points (iterable): Array of (x, y, z) locations.

        Returns:
            iterable: Array of interpolated size values.
        """
        return self._instance.InterpToPts(points)

    def set_truncation(self, maximum, minimum):
        """Clamp interpolated values to [minimum, maximum].

        Args:
            maximum (float): Upper truncation bound.
            minimum (float): Lower truncation bound.
        """
        if maximum < minimum:
            raise ValueError('The truncation maximum must be greater than minimum')
        self._instance.SetTrunc(maximum, minimum)

    @property
    def truncate_interpolation_values(self):
        """Gets the truncation interpolation values."""
        return self._instance.GetTruncateInterpolatedValues

    @property
    def truncate_min(self):
        """Gets the truncation minimum."""
        return self._instance.GetTruncMin

    @property
    def truncate_max(self):
        """Gets the truncation maximum."""
        return self._instance.GetTruncMax
