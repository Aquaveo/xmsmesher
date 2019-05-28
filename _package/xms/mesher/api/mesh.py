from xms import grid
from xms.mesher.meshing import MultiPolyMesherIo
from xms.mesher.meshing import mesh_utils


def generate_mesh(polygon_inputs, refine_points=None, **kwargs):
    # Generate Mesh
    if refine_points is None:
        refine_points = []

    mesher = MultiPolyMesherIo(polygons=polygon_inputs, refine_points=refine_points, **kwargs)

    # Generate Mesh
    success, error = mesh_utils.generate_mesh(mesher)
    if not success:
        raise RuntimeError(error)

    return grid.ugrid.UGrid(points=mesher.points, cellstream=mesher.cells)
