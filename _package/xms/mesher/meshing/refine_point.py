"""Class for representing a mesh refine point."""
from .._xmsmesher.meshing import RefinePoint as RefPoint


class RefinePoint(object):
    """Refine Point for mesh."""

    def __init__(self, point=None, size=None, create_mesh_point=True, **kwargs):
        """Constructor.

        Args:
            point: *see below*
            size:  *see below*
            create_mesh_point: *see below*
            **kwargs (dict): Generic keyword arguments
        """
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
        else:
            if point is None:
                raise ValueError("point is a required argument")
            if size is None:
                raise ValueError("size is a required argument")
            self._instance = RefPoint(point, size, create_mesh_point)

    def __eq__(self, other):
        """Equality operator.

        Args:
            other (RefinePoint): InterpLinear to compare

        Returns:
            bool: True if RefinePoints are equal
        """
        return self._instance.pt == other._instance.pt and \
            self._instance.size == other._instance.size and \
            self._instance.createMeshPoint == other._instance.createMeshPoint

    def __ne__(self, other):
        """Equality operator.

        Args:
            other (RefinePoint): RefinePoint to compare

        Returns:
            bool: True if RefinePoints are not equal
        """
        return not self.__eq__(other)

    def __str__(self):
        """Returns a string representation of the RefinePoint."""
        return "<RefinePoint: point=({}, {}, {}) size={}, create_mesh_point={}".format(
            self.point[0], self.point[1], self.point[2],
            self.size,
            self.create_mesh_point,
        )

    def __repr__(self):
        """Returns a string representation of the RefinePoint."""
        return "<RefinePoint: point=({}, {}, {}) size={}, create_mesh_point={}".format(
            self.point[0], self.point[1], self.point[2],
            self.size,
            self.create_mesh_point,
        )

    @property
    def point(self):
        """Location of the refine point."""
        return self._instance.pt

    @point.setter
    def point(self, value):
        """Set location of the refine point."""
        self._instance.pt = value

    @property
    def size(self):
        """Element size at the refine point."""
        return self._instance.size

    @size.setter
    def size(self, value):
        """Set element size at the refine point."""
        self._instance.size = value

    @property
    def create_mesh_point(self):
        """Create a mesh point at this location with attached cells at the specified size."""
        return self._instance.createMeshPoint

    @create_mesh_point.setter
    def create_mesh_point(self, value):
        """Set mesh point at this location with attached cells at the specified size."""
        self._instance.createMeshPoint = value
