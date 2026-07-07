#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup meshing
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <xmscore/misc/base_macros.h>
#include <xmscore/misc/boost_defines.h>
#include <xmsinterp/interpolate/InterpBase.h>
#include <xmscore/stl/vector.h>

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Structs / Classes ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class InterpRasterSizeFunction
/// \brief Mesh size function derived from a structured raster grid.
///
/// Stores the raster as a compact grid description (upper-left corner,
/// pixel dimensions, grid counts, and a flat row-major value array).
/// Points inside the raster extent use bilinear cell interpolation;
/// points outside are extrapolated by clamping to the nearest boundary cell.
class InterpRasterSizeFunction : public InterpBase
{
public:
  static BSHP<InterpRasterSizeFunction> New(
      double a_x0, double a_y0,
      double a_dx, double a_dy,
      int a_nx, int a_ny,
      const VecFlt& a_values,
      float a_nodata = -1.0e38f);

  virtual ~InterpRasterSizeFunction();

  /// \cond
  virtual void SetPtsTris(BSHP<VecPt3d> a_pts, BSHP<VecInt> a_tris) override = 0;
  virtual void SetScalars(const float* a_scalar, size_t a_n) override = 0;
  virtual void SetScalars(BSHP<VecFlt> a_scalar) override = 0;
  virtual float InterpToPt(const Pt3d& a_pt) override = 0;
  virtual void InterpToPts(const VecPt3d& a_pts, VecFlt& a_scalars) override = 0;
  virtual void SetPtActivity(DynBitset& a_activity) override = 0;
  virtual void SetTriActivity(DynBitset& a_activity) override = 0;
  virtual void SetTrunc(double a_sMax, double a_sMin) override = 0;

  virtual bool GetTruncateInterpolatedValues() const = 0;
  virtual double GetTruncMin() const = 0;
  virtual double GetTruncMax() const = 0;

  virtual const BSHP<VecPt3d> GetPts() const override = 0;
  virtual const BSHP<VecInt> GetTris() const override = 0;
  virtual const BSHP<VecFlt> GetScalars() const override = 0;
  virtual DynBitset GetPtActivity() const override = 0;
  virtual DynBitset GetTriActivity() const override = 0;

  virtual std::string ToString() const override = 0;
  /// \endcond

protected:
  InterpRasterSizeFunction();

private:
  XM_DISALLOW_COPY_AND_ASSIGN(InterpRasterSizeFunction);
};
//----- Function prototypes ----------------------------------------------------
} // namespace xms
