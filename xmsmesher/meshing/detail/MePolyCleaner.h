//------------------------------------------------------------------------------
/// \file
/// \ingroup meshing_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------
#pragma once

//----- Included files ---------------------------------------------------------
#include <xmscore/stl/vector.h>
#include <xmscore/misc/base_macros.h>
#include <xmscore/misc/boost_defines.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
class MePolyOffsetterOutput;

/// \brief cleans the output produced by MePolyOffsetter
/// \see MePolyCleanerImpl
class MePolyCleaner
{
public:
  static BSHP<MePolyCleaner> New();

  /// \cond
  virtual void CleanPolyOffset(const VecPt3d& a_input,
                               int a_pType,
                               double a_tol,
                               MePolyOffsetterOutput& a_out) = 0;
  virtual void SetOriginalOutsidePolygon(const VecPt3d& a_origOutPoly) = 0;

  virtual void IntersectCleanInPolys(const std::vector<MePolyOffsetterOutput>& a_offsets,
                                     MePolyOffsetterOutput& a_out,
                                     double a_xyTol) = 0;

  virtual void IntersectCleanInOutPolys(const MePolyOffsetterOutput& a_offsets,
                                        MePolyOffsetterOutput& a_out,
                                        double a_xyTol) = 0;

protected:
  MePolyCleaner() {}
  virtual ~MePolyCleaner() {}

private:
  XM_DISALLOW_COPY_AND_ASSIGN(MePolyCleaner);
  /// \endcond
};
//----- Function prototypes ----------------------------------------------------

} // namespace xms
