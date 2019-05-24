from .._xmsmesher.meshing import MultiPolyMesherIo as MesherIo
from .refine_point import RefinePoint


class MultiPolyMesherIo(object):

    def __init__(self, polygons=None, refine_points=None, check_topology=None, return_cell_polygons=None, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
        else:
            self._instance = MesherIo()
            if polygons is not None:
                self.polygons = polygons
            if refine_points is not None:
                self.refine_points = refine_points
            if check_topology is not None:
                self.check_topology = check_topology
            if return_cell_polygons is not None:
                self.return_cell_polygons = return_cell_polygons

    @property
    def polygons(self):
        """"""
        return self._instance.polys

    @polygons.setter
    def polygons(self, value):
        self._instance.polys = value

    @property
    def refine_points(self):
        """"""
        return [RefinePoint(instance=x) for x in self._instance.refPts]

    @refine_points.setter
    def refine_points(self, value):
        ref_points = [x._instance for x in value]
        self._instance.refPts = ref_points

    @property
    def check_topology(self):
        """"""
        return self._instance.checkTopology

    @check_topology.setter
    def check_topology(self, value):
        self._instance.checkTopology = value

    @property
    def return_cell_polygons(self):
        """"""
        return self._instance.returnCellPolygons

    @return_cell_polygons.setter
    def return_cell_polygons(self, value):
        self._instance.returnCellPolygons = value

    # Read only outputs
    @property
    def points(self):
        """"""
        return self._instance.points

    @property
    def cells(self):
        """"""
        return self._instance.cells

    @property
    def cell_polygons(self):
        """"""
        return self._instance.cellPolygons
