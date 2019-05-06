*********
PolyInput
*********

The PolyInput class contains information about polygons that you would like to mesh using the
:class:`MultiPolyMesherIo <xmsmesher.meshing.MultiPolyMesherIo>` class. This class defines the
polygon to be meshed. A polygon is made up of an outer polygon
(:attr:`outside_poly <xmsmesher.meshing.PolyInput.outside_poly>`), and a list of inner polygons
(:attr:`inside_polys <xmsmesher.meshing.PolyInput.inside_polys>`) representing
holes in the outer polygon. The other options available in this class help define
features of the polygons, such as :attr:`seed_poins <xmsmesher.meshing.PolyInput.seed_points>`,
:attr:`relaxation_method <xmsmesher.meshing.PolyInput.relaxation_method>`, and many more
that are described below.

.. autoclass:: xmsmesher.meshing.PolyInput
   :members:

   .. automethod:: __init__