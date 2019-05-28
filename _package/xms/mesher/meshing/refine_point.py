from .._xmsmesher.meshing import RefinePoint as RefPoint


class RefinePoint(object):

    def __init__(self, point=None, size=None, create_mesh_point=True, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
        else:
            if point is None:
                raise ValueError("point is a required argument")
            if size is None:
                raise ValueError("size is a required argument")
            self._instance = RefPoint(point, size, create_mesh_point)

    def __eq__(self, other):
        return self._instance.pt == other._instance.pt and \
               self._instance.size == other._instance.size and \
               self._instance.createMeshPoint == other._instance.createMeshPoint

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        return "<RefinePoint: point=({}, {}, {}) size={}, create_mesh_point={}".format(
            self.point[0], self.point[1], self.point[2],
            self.size,
            self.create_mesh_point,
        )

    @property
    def point(self):
        """"""
        return self._instance.pt

    @point.setter
    def point(self, value):
        self._instance.pt = value

    @property
    def size(self):
        """"""
        return self._instance.size

    @size.setter
    def size(self, value):
        self._instance.size = value

    @property
    def create_mesh_point(self):
        """"""
        return self._instance.createMeshPoint

    @create_mesh_point.setter
    def create_mesh_point(self, value):
        self._instance.createMeshPoint = value
