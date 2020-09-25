"""Test InterpLinear_py.cpp."""
import unittest

from xms import mesher

from xms.mesher.meshing import PolyInput
from xms.mesher.meshing import RefinePoint


class TestMeshApi(unittest.TestCase):
    """Tests for the xmsmesher api."""
    def test_generate_mesh_api(self):
        """Test generating a mesh using the api."""
        outside_polygon = [
            (0, 0, 0), (1, 0, 0), (2, 0, 0), (3, 0, 0),
            (3, 1, 0), (3, 2, 0), (3, 3, 0),
            (2, 3, 0), (1, 3, 0), (0, 3, 0),
            (0, 2, 0), (0, 1, 0), (0, 0, 0),
        ]
        polygon_input = PolyInput(outside_polygon=outside_polygon)
        xm_grid = mesher.generate_mesh(polygon_inputs=[polygon_input])
        self.assertEqual(20, xm_grid.cell_count)
        self.assertEqual(100, len(xm_grid.cellstream))
        self.assertEqual(((0, 0, 0), (3, 3, 0)), xm_grid.extents)

    def test_generate_mesh_api_with_refine(self):
        """Test generating a mesh with a refine point using the api."""
        outside_polygon = [
            (0, 0, 0), (1, 0, 0), (2, 0, 0), (3, 0, 0),
            (3, 1, 0), (3, 2, 0), (3, 3, 0),
            (2, 3, 0), (1, 3, 0), (0, 3, 0),
            (0, 2, 0), (0, 1, 0), (0, 0, 0),
        ]
        refine_point = RefinePoint((1.5, 1.5, 0), size=0.25)
        polygon_input = PolyInput(outside_polygon=outside_polygon)
        xm_grid = mesher.generate_mesh(polygon_inputs=[polygon_input], refine_points=[refine_point])
        self.assertEqual(50, xm_grid.cell_count)
        self.assertEqual(250, len(xm_grid.cellstream))
        self.assertEqual(((0, 0, 0), (3, 3, 0)), xm_grid.extents)

    def test_generate_mesh_api_assert(self):
        """Ensure we get expected exception with bad input."""
        outside_polygon = []
        polygon_input = PolyInput(outside_polygon=outside_polygon)

        runtime_error = "---Error: Outer polygon index 0 is empty."
        with self.assertRaises(RuntimeError) as context:
            _ = mesher.generate_mesh(polygon_inputs=[polygon_input])
        err = context.exception

        self.assertIn(runtime_error, str(err))
