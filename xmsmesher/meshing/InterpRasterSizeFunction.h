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
  virtual void SetPtsTris(BSHP<VecPt3d> a_pts, BSHP<VecInt> a_tris) override;
  virtual void SetScalars(const float* a_scalar, size_t a_n) override;
  virtual void SetScalars(BSHP<VecFlt> a_scalar) override;
  virtual float InterpToPt(const Pt3d& a_pt) override;
  virtual void InterpToPts(const VecPt3d& a_pts, VecFlt& a_scalars) override;
  virtual void SetPtActivity(DynBitset& a_activity) override;
  virtual void SetTriActivity(DynBitset& a_activity) override;
  virtual void SetTrunc(double a_sMax, double a_sMin) override;

  virtual bool GetTruncateInterpolatedValues() const;
  virtual double GetTruncMin() const;
  virtual double GetTruncMax() const;

  virtual const BSHP<VecPt3d> GetPts() const override;
  virtual const BSHP<VecInt> GetTris() const override;
  virtual const BSHP<VecFlt> GetScalars() const override;
  virtual DynBitset GetPtActivity() const override;
  virtual DynBitset GetTriActivity() const override;

  virtual std::string ToString() const override;
  /// \endcond

private:
  InterpRasterSizeFunction(double a_x0, double a_y0,
                            double a_dx, double a_dy,
                            int a_nx, int a_ny,
                            const VecFlt& a_values,
                            float a_nodata);

  void BuildCornerGeometry();

  double m_x0;        ///< X coordinate of the upper-left raster corner
  double m_y0;        ///< Y coordinate of the upper-left raster corner
  double m_dx;        ///< Pixel width (positive)
  double m_dy;        ///< Pixel height (negative for north-up rasters)
  int m_nx;           ///< Number of columns
  int m_ny;           ///< Number of rows
  float m_nodata;     ///< Nodata sentinel value (informational; not used by InterpToPt)
  bool m_truncate;    ///< Whether to clamp interpolated values
  double m_truncMin;  ///< Truncation lower bound
  double m_truncMax;  ///< Truncation upper bound
  BSHP<VecFlt>  m_values; ///< Flat row-major array of size values
  BSHP<VecPt3d> m_pts;    ///< 4 bounding-corner points
  BSHP<VecInt>  m_tris;   ///< 2 triangles covering the bounding rectangle

  XM_DISALLOW_COPY_AND_ASSIGN(InterpRasterSizeFunction);
};
//----- Function prototypes ----------------------------------------------------
} // namespace xms
