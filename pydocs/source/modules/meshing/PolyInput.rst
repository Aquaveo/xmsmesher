*********
PolyInput
*********

The PolyInput class contains information about polygons that you would like to mesh using the
:class:`MultiPolyMesherIo <xms.mesher.meshing.MultiPolyMesherIo>` class. This class defines the
polygon to be meshed. A polygon is made up of an outer polygon
(:attr:`outside_poly <xms.mesher.meshing.PolyInput.outside_poly>`), and a list of inner polygons
(:attr:`inside_polys <xms.mesher.meshing.PolyInput.inside_polys>`) representing
holes in the outer polygon. The other options available in this class help define
features of the polygons, such as :attr:`seed_poins <xms.mesher.meshing.PolyInput.seed_points>`,
:attr:`relaxation_method <xms.mesher.meshing.PolyInput.relaxation_method>`, and many more
that are described below.

.. autoclass:: xms.mesher.meshing.PolyInput
   :members: