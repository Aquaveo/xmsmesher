"""Multi-Polygon Mesher IO."""
from .poly_input import PolyInput
from .refine_point import RefinePoint
from .._xmsmesher.meshing import MultiPolyMesherIo as MesherIo


class MultiPolyMesherIo(object):
    """Multi-Polygon Mesher IO."""
    def __init__(self, polygons=None, refine_points=None, check_topology=None, return_cell_polygons=None, **kwargs):
        """Constructor.

        Args:
            polygons: *see below*
            refine_points: *see below*
            check_topology: *see below*
            return_cell_polygons: *see below*
            **kwargs (dict): Generic keyword arguments
        """
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
        """A list of :class:`xms.mesher.meshing.PolyInput` objects to be used by the meshing functions."""
        return [PolyInput(instance=x) for x in self._instance.polys]

    @polygons.setter
    def polygons(self, value):
        polygons = [v._instance for v in value]
        self._instance.polys = polygons

    @property
    def refine_points(self):
        """A list of :class:`xms.mesher.meshingRefinePoint` objects used for mesh refinement."""
        return [RefinePoint(instance=x) for x in self._instance.refPts]

    @refine_points.setter
    def refine_points(self, value):
        ref_points = [v._instance for v in value]
        self._instance.refPts = ref_points

    @property
    def check_topology(self):
        """If True, checks :class:`xms.mesher.meshing.PolyInput` topology for errors."""
        return self._instance.checkTopology

    @check_topology.setter
    def check_topology(self, value):
        self._instance.checkTopology = value

    @property
    def return_cell_polygons(self):
        """If True, the cell_polygons list will be be filled when meshing occurs."""
        return self._instance.returnCellPolygons

    @return_cell_polygons.setter
    def return_cell_polygons(self, value):
        self._instance.returnCellPolygons = value

    # Read only outputs
    @property
    def points(self):
        """A list of (x, y, z) coordinates of the resulting mesh. (Populated by meshing functions)."""
        return self._instance.points

    @property
    def cells(self):
        """A cell stream representing the mesh. (Populated by meshing functions)."""
        return self._instance.cells

    @property
    def cell_polygons(self):
        """The index of the :class:`xms.mesher.meshing.PolyInput` in cell_polygons that each cell was generated from."""
        return self._instance.cellPolygons
