**********************
PolyRedistributePoints
**********************

The PolyRedistributePts class is used to help redistribute pts on a polyline.
There are several functions on this class used to set options
(:func:`set_constant_size_bias <xms.mesher.meshing.PolyRedistributePts.set_constant_size_bias>`,
:func:`set_constant_size_func <xms.mesher.meshing.PolyRedistributePts.set_constant_size_func>`,
:func:`set_size_func <xms.mesher.meshing.PolyRedistributePts.set_size_func>`) as well as the
function :func:`redistribute <xms.mesher.meshing.PolyRedistributePts.redistribute>` that actaully does all the work.

Example:

.. code-block:: python

    from xms.mesher.meshing import PolyRedistributePoints

    polygon_corners = [(0, 0, 0), (0, 100, 0), (100, 100, 0),
                       (100, 0, 0), (0, 0, 0)]
    rp = PolyRedistributePoints()
    rp.set_constant_size_function(10)
    redistributed_points = rp.redistribute(polygon_corners)


.. autoclass:: xms.mesher.meshing.PolyRedistributePoints
   :members:
