"""Test InterpRasterSizeFunction_py.cpp."""
import unittest

import numpy as np

from xms.mesher.meshing import InterpRasterSizeFunction


class TestInterpRasterSizeFunction(unittest.TestCase):
    """Test InterpRasterSizeFunction Class."""

    def setUp(self):
        """Set up for each test case."""
        # 2x2 raster, north-up (dy < 0), origin at upper-left corner (0, 10)
        self.values = (1.0, 2.0, 3.0, 4.0)
        self.interp = InterpRasterSizeFunction(0.0, 10.0, 5.0, -5.0, 2, 2, self.values)

    def test_interp_to_pt_on_cell_center(self):
        """Interpolate to a point that lands exactly on a raster cell."""
        val = self.interp.interpolate_to_point((0.0, 10.0, 0.0))
        self.assertEqual(1.0, val)

    def test_interp_to_pts(self):
        """Interpolate to multiple points."""
        pts = ((0.0, 10.0, 0.0), (5.0, 10.0, 0.0))
        ret = self.interp.interpolate_to_points(pts)
        np.testing.assert_array_almost_equal((1.0, 2.0), ret)

    def test_set_truncation(self):
        """Test set_truncation and the truncation getters."""
        t_min = 1.5
        t_max = 3.5

        self.assertFalse(self.interp.truncate_interpolation_values)

        self.interp.set_truncation(t_max, t_min)

        self.assertTrue(self.interp.truncate_interpolation_values)
        self.assertEqual(t_min, self.interp.truncate_min)
        self.assertEqual(t_max, self.interp.truncate_max)

    def test_set_truncation_max_less_than_min_raises(self):
        """Test set_truncation raises when maximum < minimum."""
        with self.assertRaises(ValueError):
            self.interp.set_truncation(1.0, 2.0)

    def test_mismatched_values_length_raises(self):
        """Test constructing with a values array of the wrong length raises."""
        with self.assertRaises(ValueError):
            InterpRasterSizeFunction(0.0, 10.0, 5.0, -5.0, 2, 2, (1.0, 2.0, 3.0))

    def test_non_positive_grid_size_raises(self):
        """Test constructing with non-positive nx/ny raises."""
        with self.assertRaises(ValueError):
            InterpRasterSizeFunction(0.0, 10.0, 5.0, -5.0, 0, 2, ())

    def test_zero_pixel_size_raises(self):
        """Test constructing with a zero dx/dy raises."""
        with self.assertRaises(ValueError):
            InterpRasterSizeFunction(0.0, 10.0, 0.0, -5.0, 2, 2, self.values)

    def test_instance_kwarg_round_trip(self):
        """Test constructing from an existing instance (used by round-trip wrappers)."""
        wrapped = InterpRasterSizeFunction(instance=self.interp._instance)
        self.assertEqual(str(self.interp), str(wrapped))
        self.assertEqual(self.interp, wrapped)

    def test_equality(self):
        """Test __eq__/__ne__."""
        other = InterpRasterSizeFunction(0.0, 10.0, 5.0, -5.0, 2, 2, self.values)
        self.assertNotEqual(self.interp, other)
        self.assertNotEqual(self.interp, "not an interpolator")

    def test_str(self):
        """Test the string representation."""
        self.assertIn('InterpRasterSizeFunction', str(self.interp))
        self.assertIn('InterpRasterSizeFunction', repr(self.interp))


if __name__ == '__main__':
    unittest.main()
