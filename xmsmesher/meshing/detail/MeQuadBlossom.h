#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup meshing
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard library headers
#include <vector>

// 4. External library headers

// 5. Shared code headers
#include <xmscore/misc/base_macros.h>
#include <xmscore/misc/boost_defines.h>
#include <xmscore/stl/vector.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Forward declarations ---------------------------------------------------
class XmUGrid;

//----- Structs / Classes ------------------------------------------------------

class MeQuadBlossom
{
public:
  static BSHP<MeQuadBlossom> New(std::shared_ptr<XmUGrid> a_ugrid);
  MeQuadBlossom();
  virtual ~MeQuadBlossom();

  /// \cond
  virtual int PreMakeQuads() = 0;
  virtual std::shared_ptr<XmUGrid> MakeQuads(bool a_splitBoundaryPoints, bool a_useAngle) = 0;
  /// \endcond

  static double EstimatedRunTimeInMinutes(int a_numPoints);
  static std::shared_ptr<XmUGrid> SplitToQuads(std::shared_ptr<XmUGrid> a_ugrid);

private:
  /// \cond
  XM_DISALLOW_COPY_AND_ASSIGN(MeQuadBlossom);
  /// \endcond
}; // class MeQuadBlossom

//----- Function prototypes ----------------------------------------------------

} // namespace xms
