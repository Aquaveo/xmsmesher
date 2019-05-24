"""Test MultiPolyMesherIo_py.cpp."""
import unittest
import numpy as np
from xms.mesher.meshing import MultiPolyMesherIo
from xms.mesher.meshing import PolyInput
from xms.mesher.meshing import RefinePoint
from xms.interp.interpolate import InterpLinear
from xms.interp.interpolate import InterpIdw


class TestMultiPolyMesherIo(unittest.TestCase):
    """Test MultiPolyMesherIo functions."""

    def setUp(self):
        pass

    def assertInsidePolysEqual(self, base, out):
        self.assertEqual(len(base), len(out), "Base InsidePolys and Out InsidePolys lengths do not match.")
        for i in range(len(base)):
            np.testing.assert_array_equal(base[i], out[i],
                                          "inside_poly[{}] is not the same, -- {} != {}".format(i, base[i], out[i]))

    @staticmethod
    def assertArraysEqual(base, out):
        np.testing.assert_array_equal(np.array(base), out)

    def assertTupleStringsEqual(self, base, out):
        self.assertEqual(len(base), len(out), "Base PolyInputs and Out PolyInputs lengths do not match.")
        for i in range(0, len(base)):
            self.assertEqual(str(base[i]), str(out[i]))


    def test_creating_MultiPolyMesherIo(self):
        io = MultiPolyMesherIo(())
        self.assertIsInstance(io, MultiPolyMesherIo)
        self.assertEqual(False, io.check_topology)
        self.assertEqual(True, io.return_cell_polygons)
        self.assertEqual(0, len(io.points))
        self.assertEqual(0, len(io.cells))
        self.assertEqual(0, len(io.cell_polygons))
        self.assertEqual(0, len(io.polygons))
        self.assertEqual(0, len(io.refine_points))

    def test_properties_MultiPolyMesherIo(self):
        io = MultiPolyMesherIo(())
        self.assertIsInstance(io, MultiPolyMesherIo)

        io.check_topology = True
        self.assertEqual(True, io.check_topology)

        io.return_cell_polygons = False
        self.assertEqual(False, io.return_cell_polygons)

        out_poly = ((0, 0, 0), (1, 0, 0), (1, 1, 0), (0, 1, 0))
        pi1 = PolyInput(out_poly)
        pi1.bias = 2.718
        pi2 = PolyInput(out_poly)
        pi2.bias = 0.618
        io.polygon_inputs = (pi1, pi2)
        self.assertTupleStringsEqual((pi1, pi2), io.polygon_inputs)

        rp1 = RefinePoint((5, 0, -3), 3.1, False)
        rp2 = RefinePoint((-2, -2, 1), -0.4, True)
        ref_points = [rp1, rp2]
        io.refine_points = ref_points
        io_refine_points = io.refine_points
        for i in range(len(io_refine_points)):
            self.assertTrue(ref_points[i] == io_refine_points[i])

class TestPolyInput(unittest.TestCase):
    """Test PolyInput functions."""

    def setUp(self):
        pass

    def assertInsidePolysEqual(self, base, out):
        self.assertEqual(len(base), len(out), "Base InsidePolys and Out InsidePolys lengths do not match.")
        for i in range(len(base)):
            np.testing.assert_array_equal(base[i], out[i],
                                          "inside_poly[{}] is not the same, -- {} != {}".format(i, base[i], out[i]))

    @staticmethod
    def assertArraysEqual(base, out):
        np.testing.assert_array_equal(np.array(base), out)

    def test_creating_default_PolyInput(self):
        out_poly = ((0, 0, 0), (1, 0, 0), (1, 1, 0), (0, 1, 0))
        pi = PolyInput(out_poly)
        self.assertIsInstance(pi, PolyInput)
        self.assertEqual(4, len(pi.outside_polygon))
        self.assertEqual(0, len(pi.inside_polygons))
        self.assertEqual(1.0, pi.bias)
        self.assertEqual(None, pi.size_function)
        self.assertEqual(None, pi.elevation_function)
        self.assertEqual(-1, pi.constant_size_bias)
        self.assertEqual(-1, pi.constant_size_function)
        self.assertEqual(False, pi.remove_internal_four_triangle_points)

    def test_creating_PolyInput(self):
        outside_poly = ((1, 2, 0), (5, 2, 0), (5, 9, 0), (1, 9, 0))
        inside_polys = (((3, 3, 0), (2.5, 4, 0), (2, 3, 0)),
                        ((4, 8, 0), (3, 7, 0), (2, 8, 0)))
        poly_corners = (0, 1, 2, 3)
        bias = 3.14159
        pts = ((1, 0, 0), (10, 0, 0), (10, 10, 0))
        size_func = InterpLinear(pts)
        elev_func = InterpIdw(pts)

        pi = PolyInput(outside_polygon=outside_poly, inside_polygons=inside_polys,
                       bias=bias, size_function=size_func, polygon_corners=poly_corners,
                       elevation_function=elev_func)
        self.assertIsInstance(pi, PolyInput)
        self.assertArraysEqual(outside_poly, pi.outside_polygon)
        self.assertInsidePolysEqual(inside_polys, pi.inside_polygons)
        self.assertEqual(bias, pi.bias)
        # self.assertEqual(size_func.to_string(), pi.size_function.to_string())
        # self.assertEqual(elev_func.to_string(), pi.elevation_function.to_string())
        self.assertEqual(-1, pi.constant_size_bias)
        self.assertEqual(-1, pi.constant_size_function)
        self.assertEqual(False, pi.remove_internal_four_triangle_points)

    def test_properties_PolyInput(self):
        out_poly = ((0, 0, 0), (1, 0, 0), (1, 1, 0), (0, 1, 0))
        pi = PolyInput(out_poly)
        outside_poly = ((1, 2, 0), (5, 2, 0), (5, 9, 0), (1, 9, 0))
        inside_polys = (((3, 3, 0), (2.5, 4, 0), (2, 3, 0)),
                        ((4, 8, 0), (3, 7, 0), (2, 8, 0)))
        poly_corners = (0, 1, 2, 3)
        pts = ((1, 2, 3), (4, 5, 6), (0, 5, 7))
        size_func = InterpLinear(pts)
        elev_func = InterpIdw(pts)
        seed_points = ((3, 3, 0), (4, 3, 0), (4, 8, 0), (3, 8, 0))
        relaxation_method = "spring_relaxation"

        self.assertEqual(4, len(pi.outside_polygon))
        pi.outside_polygon = outside_poly
        self.assertArraysEqual(outside_poly, pi.outside_polygon)

        self.assertEqual(0, len(pi.inside_polygons))
        pi.inside_polygons = inside_polys
        self.assertInsidePolysEqual(inside_polys, pi.inside_polygons)

        self.assertEqual(0, len(pi.polygon_corners))
        pi.polygon_corners = poly_corners
        self.assertArraysEqual(poly_corners, pi.polygon_corners)

        self.assertEqual(1.0, pi.bias)
        pi.bias = 0.3
        self.assertEqual(0.3, pi.bias)

        self.assertEqual(None, pi.size_function)
        pi.size_function = size_func
        #self.assertEqual(str(size_func), str(pi.size_function))

        self.assertEqual(None, pi.elevation_function)
        pi.elevation_function = elev_func
        #self.assertEqual(elev_func.to_string(), pi.elevation_function.to_string())

        self.assertEqual(-1, pi.constant_size_bias)
        pi.constant_size_bias = 4.0
        self.assertEqual(4.0, pi.constant_size_bias)

        self.assertEqual(-1, pi.constant_size_function)
        pi.constant_size_function = 1.2
        self.assertEqual(1.2, pi.constant_size_function)

        self.assertEqual(False, pi.remove_internal_four_triangle_points)
        pi.remove_internal_four_triangle_points = True
        self.assertEqual(True, pi.remove_internal_four_triangle_points)

        self.assertEqual(0, len(pi.seed_points))
        pi.seed_points = seed_points
        self.assertArraysEqual(seed_points, pi.seed_points)

        self.assertEqual("", pi.relaxation_method)
        pi.relaxation_method = relaxation_method
        self.assertEqual(relaxation_method, pi.relaxation_method)

class TestRefinePoint(unittest.TestCase):
    """Test RefinePoint functions."""

    def setUp(self):
        pass

    def test_creating_RefinePoint(self):
        rp = RefinePoint((1, 2, 3), -2.0, True)
        self.assertIsInstance(rp, RefinePoint)
        self.assertEqual((1, 2, 3), rp.point)
        self.assertEqual(-2.0, rp.size)
        self.assertEqual(True, rp.create_mesh_point)

        rp2 = RefinePoint(point=(1, 1, 3), create_mesh_point=False, size=2.0)
        self.assertIsInstance(rp2, RefinePoint)
        self.assertEqual((1, 1, 3), rp2.point)
        self.assertEqual(2.0, rp2.size)
        self.assertEqual(False, rp2.create_mesh_point)

    def test_properties_RefinePoint(self):
        rp = RefinePoint((4, 5, 3), 2.0, False)

        self.assertEqual(2.0, rp.size)
        rp.size = 4.5
        self.assertEqual(4.5, rp.size)

        self.assertEqual(False, rp.create_mesh_point)
        rp.create_mesh_point = True
        self.assertEqual(True, rp.create_mesh_point)

        self.assertEqual((4, 5, 3), rp.point)
        rp.point = (-2.0, 4.15, -900.001)
        self.assertEqual((-2.0, 4.15, -900.001), rp.point)

