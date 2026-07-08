//------------------------------------------------------------------------------
/// \file
/// \ingroup meshing
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My header
#include <xmsmesher/meshing/InterpRasterSizeFunction.h>

// 3. Standard Library Headers
#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

// 4. External Library Headers

// 5. Shared Headers
#include <xmscore/misc/boost_defines.h>
#include <xmscore/misc/DynBitset.h>
#include <xmscore/misc/xmstype.h>
#include <xmscore/points/pt.h>
#include <xmscore/stl/vector.h>

// 6. Non-shared Headers

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Classes / Structs ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \brief Implementation of InterpRasterSizeFunction
class InterpRasterSizeFunctionImpl : public InterpRasterSizeFunction
{
public:
  InterpRasterSizeFunctionImpl(double a_x0, double a_y0,
                               double a_dx, double a_dy,
                               int a_nx, int a_ny,
                               const VecFlt& a_values,
                               float a_nodata)
  : m_x0(a_x0)
  , m_y0(a_y0)
  , m_dx(a_dx)
  , m_dy(a_dy)
  , m_nx(a_nx)
  , m_ny(a_ny)
  , m_nodata(a_nodata)
  , m_truncate(false)
  , m_truncMin(0.0)
  , m_truncMax(0.0)
  {
    m_values = BSHP<VecFlt>(new VecFlt(a_values));
    BuildCornerGeometry();
  }

  /// Build the 4-corner pts and 2-triangle tris that bound the raster.
  void BuildCornerGeometry()
  {
    double xRight = m_x0 + m_nx * m_dx;
    double yFar   = m_y0 + m_ny * m_dy; // < m_y0 for north-up (dy < 0)

    m_pts = BSHP<VecPt3d>(new VecPt3d);
    m_pts->push_back(Pt3d(m_x0,   m_y0,  0.0)); // 0: near-left
    m_pts->push_back(Pt3d(xRight, m_y0,  0.0)); // 1: near-right
    m_pts->push_back(Pt3d(xRight, yFar,  0.0)); // 2: far-right
    m_pts->push_back(Pt3d(m_x0,   yFar,  0.0)); // 3: far-left

    m_tris = BSHP<VecInt>(new VecInt);
    m_tris->push_back(0); m_tris->push_back(1); m_tris->push_back(2);
    m_tris->push_back(0); m_tris->push_back(2); m_tris->push_back(3);
  }

  void SetPtsTris(BSHP<VecPt3d> /*a_pts*/, BSHP<VecInt> /*a_tris*/) override {}

  void SetScalars(const float* a_scalar, size_t a_n) override
  {
    m_values = BSHP<VecFlt>(new VecFlt(a_scalar, a_scalar + a_n));
  }

  void SetScalars(BSHP<VecFlt> a_scalar) override
  {
    m_values = a_scalar;
  }

  float InterpToPt(const Pt3d& a_pt) override
  {
    // Fractional column and row indices from the upper-left corner.
    // For north-up rasters m_dy < 0, so row_f increases downward.
    double col_f = (a_pt.x - m_x0) / m_dx;
    double row_f = (a_pt.y - m_y0) / m_dy;

    float result;

    bool inside = col_f >= 0.0 && col_f < static_cast<double>(m_nx)
               && row_f >= 0.0 && row_f < static_cast<double>(m_ny);

    if (inside)
    {
      // Bilinear interpolation between the four surrounding cells.
      int c0 = static_cast<int>(col_f);
      int r0 = static_cast<int>(row_f);
      int c1 = std::min(c0 + 1, m_nx - 1);
      int r1 = std::min(r0 + 1, m_ny - 1);
      float fc = static_cast<float>(col_f - c0);
      float fr = static_cast<float>(row_f - r0);

      float v00 = (*m_values)[r0 * m_nx + c0];
      float v01 = (*m_values)[r0 * m_nx + c1];
      float v10 = (*m_values)[r1 * m_nx + c0];
      float v11 = (*m_values)[r1 * m_nx + c1];

      result = v00 * (1.0f - fc) * (1.0f - fr)
             + v01 * fc           * (1.0f - fr)
             + v10 * (1.0f - fc) * fr
             + v11 * fc           * fr;
    }
    else
    {
      // Nearest boundary cell — clamp fractional indices into valid range.
      int col = std::max(0, std::min(m_nx - 1, static_cast<int>(std::floor(col_f))));
      int row = std::max(0, std::min(m_ny - 1, static_cast<int>(std::floor(row_f))));
      result = (*m_values)[row * m_nx + col];
    }

    if (m_truncate)
    {
      result = std::max(static_cast<float>(m_truncMin),
                        std::min(static_cast<float>(m_truncMax), result));
    }

    return result;
  }

  void InterpToPts(const VecPt3d& a_pts, VecFlt& a_scalars) override
  {
    a_scalars.resize(a_pts.size());
    for (size_t i = 0; i < a_pts.size(); ++i)
      a_scalars[i] = InterpToPt(a_pts[i]);
  }

  void SetPtActivity(DynBitset& /*a_activity*/) override {}
  void SetTriActivity(DynBitset& /*a_activity*/) override {}

  void SetTrunc(double a_sMax, double a_sMin) override
  {
    m_truncate = true;
    m_truncMax = a_sMax;
    m_truncMin = a_sMin;
  }

  bool GetTruncateInterpolatedValues() const override { return m_truncate; }
  double GetTruncMin() const override { return m_truncMin; }
  double GetTruncMax() const override { return m_truncMax; }

  const BSHP<VecPt3d> GetPts() const override { return m_pts; }
  const BSHP<VecInt>  GetTris() const override { return m_tris; }
  const BSHP<VecFlt>  GetScalars() const override { return m_values; }
  DynBitset GetPtActivity() const override { return DynBitset(); }
  DynBitset GetTriActivity() const override { return DynBitset(); }

  std::string ToString() const override
  {
    std::ostringstream ss;
    ss << "InterpRasterSizeFunction"
       << " origin=(" << m_x0 << "," << m_y0 << ")"
       << " dx=" << m_dx << " dy=" << m_dy
       << " nx=" << m_nx << " ny=" << m_ny;
    return ss.str();
  }

private:
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
};

//----- Static factory ---------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \brief Creates a new InterpRasterSizeFunction.
/// \param[in] a_x0    X of the upper-left raster corner in display CRS.
/// \param[in] a_y0    Y of the upper-left raster corner in display CRS.
/// \param[in] a_dx    Pixel width (positive).
/// \param[in] a_dy    Pixel height (negative for north-up rasters).
/// \param[in] a_nx    Number of columns.
/// \param[in] a_ny    Number of rows.
/// \param[in] a_values Flat row-major array of size values; length must be a_nx * a_ny.
/// \param[in] a_nodata Nodata sentinel (informational only).
/// \return Shared pointer to InterpRasterSizeFunction.
BSHP<InterpRasterSizeFunction> InterpRasterSizeFunction::New(
    double a_x0, double a_y0,
    double a_dx, double a_dy,
    int a_nx, int a_ny,
    const VecFlt& a_values,
    float a_nodata)
{
  if (a_nx <= 0 || a_ny <= 0)
    throw std::invalid_argument("InterpRasterSizeFunction: nx and ny must be positive.");
  if (a_dx == 0.0 || a_dy == 0.0)
    throw std::invalid_argument("InterpRasterSizeFunction: dx and dy must be nonzero.");
  if (a_values.size() != static_cast<size_t>(a_nx) * static_cast<size_t>(a_ny))
    throw std::invalid_argument("InterpRasterSizeFunction: values.size() must equal nx * ny.");

  BSHP<InterpRasterSizeFunctionImpl> impl(
    new InterpRasterSizeFunctionImpl(a_x0, a_y0, a_dx, a_dy, a_nx, a_ny, a_values, a_nodata));
  return BDPC<InterpRasterSizeFunction>(impl);
}

InterpRasterSizeFunction::InterpRasterSizeFunction() {}
InterpRasterSizeFunction::~InterpRasterSizeFunction() {}

} // namespace xms

#ifdef CXX_TEST
////////////////////////////////////////////////////////////////////////////////

#include <xmsmesher/meshing/InterpRasterSizeFunction.t.h>

#include <xmscore/testing/TestTools.h>

using namespace xms;

namespace
{
//------------------------------------------------------------------------------
/// \brief Builds a 2x2 north-up raster with corner values 1,2 (top) / 3,4 (bottom).
///
/// Origin (0, 10) is the upper-left corner, 5x5 pixels, so the raster spans
/// x in [0, 10] and y in [0, 10].
//------------------------------------------------------------------------------
BSHP<InterpRasterSizeFunction> iNew2x2Raster()
{
  VecFlt values = {1.0f, 2.0f, 3.0f, 4.0f};
  return InterpRasterSizeFunction::New(0.0, 10.0, 5.0, -5.0, 2, 2, values);
} // iNew2x2Raster
//------------------------------------------------------------------------------
/// \brief Returns true if a_fn threw std::invalid_argument, false otherwise.
///
/// CxxTest's TS_ASSERT_THROWS relies on _CXXTEST_HAVE_EH, which isn't defined in
/// this build, so its generated catch block is empty; use a plain try/catch instead.
//------------------------------------------------------------------------------
template <typename Fn>
bool iThrowsInvalidArgument(Fn a_fn)
{
  try
  {
    a_fn();
  }
  catch (const std::invalid_argument&)
  {
    return true;
  }
  return false;
} // iThrowsInvalidArgument
} // unnamed namespace

////////////////////////////////////////////////////////////////////////////////
/// \class InterpRasterSizeFunctionUnitTests
/// \brief tester for the InterpRasterSizeFunction class
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief tests that a valid raster description constructs successfully
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testNewValidConstruction()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();
  TS_ASSERT(interp);
  std::string s = interp->ToString();
  TS_ASSERT(s.find("InterpRasterSizeFunction") != std::string::npos);
} // InterpRasterSizeFunctionUnitTests::testNewValidConstruction
//------------------------------------------------------------------------------
/// \brief tests that New throws when nx is not positive
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testNewThrowsOnNonPositiveNx()
{
  VecFlt values;
  TS_ASSERT(iThrowsInvalidArgument(
    [&]() { InterpRasterSizeFunction::New(0.0, 10.0, 5.0, -5.0, 0, 2, values); }));
} // InterpRasterSizeFunctionUnitTests::testNewThrowsOnNonPositiveNx
//------------------------------------------------------------------------------
/// \brief tests that New throws when ny is not positive
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testNewThrowsOnNonPositiveNy()
{
  VecFlt values;
  TS_ASSERT(iThrowsInvalidArgument(
    [&]() { InterpRasterSizeFunction::New(0.0, 10.0, 5.0, -5.0, 2, 0, values); }));
} // InterpRasterSizeFunctionUnitTests::testNewThrowsOnNonPositiveNy
//------------------------------------------------------------------------------
/// \brief tests that New throws when dx is zero
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testNewThrowsOnZeroDx()
{
  VecFlt values = {1.0f, 2.0f, 3.0f, 4.0f};
  TS_ASSERT(iThrowsInvalidArgument(
    [&]() { InterpRasterSizeFunction::New(0.0, 10.0, 0.0, -5.0, 2, 2, values); }));
} // InterpRasterSizeFunctionUnitTests::testNewThrowsOnZeroDx
//------------------------------------------------------------------------------
/// \brief tests that New throws when dy is zero
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testNewThrowsOnZeroDy()
{
  VecFlt values = {1.0f, 2.0f, 3.0f, 4.0f};
  TS_ASSERT(iThrowsInvalidArgument(
    [&]() { InterpRasterSizeFunction::New(0.0, 10.0, 5.0, 0.0, 2, 2, values); }));
} // InterpRasterSizeFunctionUnitTests::testNewThrowsOnZeroDy
//------------------------------------------------------------------------------
/// \brief tests that New throws when values.size() != nx * ny
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testNewThrowsOnMismatchedValuesSize()
{
  VecFlt values = {1.0f, 2.0f, 3.0f};
  TS_ASSERT(iThrowsInvalidArgument(
    [&]() { InterpRasterSizeFunction::New(0.0, 10.0, 5.0, -5.0, 2, 2, values); }));
} // InterpRasterSizeFunctionUnitTests::testNewThrowsOnMismatchedValuesSize
//------------------------------------------------------------------------------
/// \brief tests the 4-corner points and 2 bounding triangles
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testGetPtsAndTrisBoundingGeometry()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();

  VecPt3d basePts = {{0.0, 10.0, 0.0}, {10.0, 10.0, 0.0}, {10.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
  TS_ASSERT_DELTA_VECPT3D(basePts, *interp->GetPts(), 1e-9);

  VecInt baseTris = {0, 1, 2, 0, 2, 3};
  TS_ASSERT_EQUALS_VEC(baseTris, (*interp->GetTris()));
} // InterpRasterSizeFunctionUnitTests::testGetPtsAndTrisBoundingGeometry
//------------------------------------------------------------------------------
/// \brief tests interpolating exactly on the upper-left raster corner
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testInterpToPtOnGridCorner()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();
  TS_ASSERT_EQUALS(1.0f, interp->InterpToPt(Pt3d(0.0, 10.0, 0.0)));
  TS_ASSERT_EQUALS(2.0f, interp->InterpToPt(Pt3d(5.0, 10.0, 0.0)));
} // InterpRasterSizeFunctionUnitTests::testInterpToPtOnGridCorner
//------------------------------------------------------------------------------
/// \brief tests bilinear interpolation at the center of the 4 raster cells
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testInterpToPtBilinearInterior()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();
  // Center of the 2x2 grid is the average of all 4 corner values: (1+2+3+4)/4.
  TS_ASSERT_DELTA(2.5, interp->InterpToPt(Pt3d(2.5, 7.5, 0.0)), 1e-6);
} // InterpRasterSizeFunctionUnitTests::testInterpToPtBilinearInterior
//------------------------------------------------------------------------------
/// \brief tests that points outside the raster clamp to the nearest boundary cell
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testInterpToPtOutsideClampsToNearestCell()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();
  // Far outside to the upper-left -> clamps to the top-left cell value.
  TS_ASSERT_EQUALS(1.0f, interp->InterpToPt(Pt3d(-100.0, 200.0, 0.0)));
  // Far outside to the lower-right -> clamps to the bottom-right cell value.
  TS_ASSERT_EQUALS(4.0f, interp->InterpToPt(Pt3d(1000.0, -1000.0, 0.0)));
  // Outside only in x, inside in y -> clamps column only.
  TS_ASSERT_EQUALS(2.0f, interp->InterpToPt(Pt3d(1000.0, 10.0, 0.0)));
  // Far outside to the lower-left (west + south) -> column and row clamp to opposite
  // extremes from the two corner cases above, landing on the bottom-left cell value.
  TS_ASSERT_EQUALS(3.0f, interp->InterpToPt(Pt3d(-1000.0, -1000.0, 0.0)));
} // InterpRasterSizeFunctionUnitTests::testInterpToPtOutsideClampsToNearestCell
//------------------------------------------------------------------------------
/// \brief tests interpolating to multiple points at once
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testInterpToPts()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();
  VecPt3d pts = {{0.0, 10.0, 0.0}, {5.0, 10.0, 0.0}, {2.5, 7.5, 0.0}};
  VecFlt scalars;
  interp->InterpToPts(pts, scalars);

  VecFlt baseScalars = {1.0f, 2.0f, 2.5f};
  TS_ASSERT_DELTA_VEC(baseScalars, scalars, 1e-6);
} // InterpRasterSizeFunctionUnitTests::testInterpToPts
//------------------------------------------------------------------------------
/// \brief tests that truncation is off by default
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testSetTruncDefaultsToOff()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();
  TS_ASSERT_EQUALS(false, interp->GetTruncateInterpolatedValues());
  TS_ASSERT_EQUALS(0.0, interp->GetTruncMin());
  TS_ASSERT_EQUALS(0.0, interp->GetTruncMax());
} // InterpRasterSizeFunctionUnitTests::testSetTruncDefaultsToOff
//------------------------------------------------------------------------------
/// \brief tests that SetTrunc clamps interpolated values to [min, max]
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testSetTruncClampsInterpolatedValue()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();
  interp->SetTrunc(/*a_sMax*/ 2.0, /*a_sMin*/ 1.5);

  TS_ASSERT_EQUALS(true, interp->GetTruncateInterpolatedValues());
  TS_ASSERT_EQUALS(1.5, interp->GetTruncMin());
  TS_ASSERT_EQUALS(2.0, interp->GetTruncMax());

  // Raw center value is 2.5, clamped down to the truncation max of 2.0.
  TS_ASSERT_EQUALS(2.0f, interp->InterpToPt(Pt3d(2.5, 7.5, 0.0)));
  // Raw top-left value is 1.0, clamped up to the truncation min of 1.5.
  TS_ASSERT_EQUALS(1.5f, interp->InterpToPt(Pt3d(0.0, 10.0, 0.0)));
} // InterpRasterSizeFunctionUnitTests::testSetTruncClampsInterpolatedValue
//------------------------------------------------------------------------------
/// \brief tests replacing the raster values with a raw pointer
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testSetScalarsFromPointer()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();
  float newValues[4] = {10.0f, 20.0f, 30.0f, 40.0f};
  interp->SetScalars(newValues, 4);

  VecFlt baseScalars = {10.0f, 20.0f, 30.0f, 40.0f};
  TS_ASSERT_DELTA_VEC(baseScalars, *interp->GetScalars(), 1e-6);
  TS_ASSERT_EQUALS(10.0f, interp->InterpToPt(Pt3d(0.0, 10.0, 0.0)));
} // InterpRasterSizeFunctionUnitTests::testSetScalarsFromPointer
//------------------------------------------------------------------------------
/// \brief tests replacing the raster values with a shared vector
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testSetScalarsFromSharedPtr()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();
  BSHP<VecFlt> newValues(new VecFlt({10.0f, 20.0f, 30.0f, 40.0f}));
  interp->SetScalars(newValues);

  TS_ASSERT(newValues == interp->GetScalars());
} // InterpRasterSizeFunctionUnitTests::testSetScalarsFromSharedPtr
//------------------------------------------------------------------------------
/// \brief tests that point/triangle activity are unused and return empty
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testPtAndTriActivityAreEmpty()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();
  DynBitset activity;
  activity.resize(4, true);

  interp->SetPtActivity(activity);
  interp->SetTriActivity(activity);

  TS_ASSERT_EQUALS((size_t)0, interp->GetPtActivity().size());
  TS_ASSERT_EQUALS((size_t)0, interp->GetTriActivity().size());
} // InterpRasterSizeFunctionUnitTests::testPtAndTriActivityAreEmpty
//------------------------------------------------------------------------------
/// \brief tests that SetPtsTris does not affect the bounding-corner geometry
//------------------------------------------------------------------------------
void InterpRasterSizeFunctionUnitTests::testSetPtsTrisIsNoOp()
{
  BSHP<InterpRasterSizeFunction> interp = iNew2x2Raster();
  VecPt3d baseGeomPts(*interp->GetPts());
  VecInt baseGeomTris(*interp->GetTris());

  BSHP<VecPt3d> otherPts(new VecPt3d({{99.0, 99.0, 0.0}}));
  BSHP<VecInt> otherTris(new VecInt({0, 0, 0}));
  interp->SetPtsTris(otherPts, otherTris);

  TS_ASSERT_DELTA_VECPT3D(baseGeomPts, *interp->GetPts(), 1e-9);
  TS_ASSERT_EQUALS_VEC(baseGeomTris, (*interp->GetTris()));
} // InterpRasterSizeFunctionUnitTests::testSetPtsTrisIsNoOp

#endif
