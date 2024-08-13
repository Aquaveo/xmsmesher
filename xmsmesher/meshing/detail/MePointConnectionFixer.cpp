//------------------------------------------------------------------------------
/// \file
/// \ingroup meshing_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsmesher/meshing/detail/MePointConnectionFixer.h>

// 3. Standard library headers
#include <map>
//#include <cmath>
//#include <numeric>

// 4. External library headers
//#include <boost/utility.hpp>
#include <xmsgrid/geometry/geoms.h>

// 5. Shared code headers
#include <xmscore/math/math.h>
#include <xmscore/misc/XmError.h>
//#include <xmscore/misc/xmstype.h>
#include <xmsgrid/triangulate/TrTin.h>
#include <xmsgrid/triangulate/TrTriangulatorPoints.h>

// 6. Non-shared code headers
#include <xmsmesher/meshing/MePolyMesher.h>
#include <xmsmesher/meshing/MeMultiPolyMesherIo.h>



//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

namespace
{

class MePointConnectionFixerImpl : public MePointConnectionFixer
{
public:
  MePointConnectionFixerImpl(BSHP<TrTin> a_tin);

  virtual BSHP<TrTin> Fix(const VecInt& a_noDeletePts) override;

  void CalcPolygonFromPointIndex();
  void CreateNewPoints();
  void CreateNewTin();

private:
  BSHP<TrTin> m_tin;           ///< The input TIN.
  BSHP<TrTin> m_outTin;        ///< The output TIN.
  VecInt m_tris;               ///< Triangles in the TIN.
  VecInt2d m_adjTris;          ///< Triangles adjacent to points.
  std::set<int> m_noDeletePts; ///< Point indexes that may not be deleted.
  std::set<int> m_removePts;   ///< Point indexes that will be removed
  VecPt3d m_newPts;            ///< Points that will be added to the TIN
  int m_ptIdx;                 ///< Index of point to be removed
  VecInt m_poly;               ///< Point indexes that make a polygon around a point being removed
};

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class MePointConnectionFixerImpl
/// \brief Identifies and removes points that are connected to more than 7 cells
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Constructor.
/// \param[in] a_tin The TIN to fix points with more than 7 connections.
//------------------------------------------------------------------------------
MePointConnectionFixerImpl::MePointConnectionFixerImpl(BSHP<TrTin> a_tin)
: m_tin(a_tin)
, m_outTin()
, m_tris()
, m_adjTris()
, m_noDeletePts()
, m_removePts()
, m_newPts()
, m_poly()
, m_ptIdx(-1)
{
} // MePointConnectionFixerImpl::MePointConnectionFixerImpl
//------------------------------------------------------------------------------
/// \brief Fixes points that have more than 7 connections.
/// \param[in] a_noDeletePts Point indexes to points that cannot be removed.
/// \return a new TIN with the points replaced that had more than 7 connections
//------------------------------------------------------------------------------
BSHP<TrTin> MePointConnectionFixerImpl::Fix(const VecInt& a_noDeletePts)
{
  BSHP<TrTin> rval = m_tin;
  XM_ENSURE_TRUE(m_tin, rval);
  XM_ENSURE_TRUE(!m_tin->Triangles().empty(), rval);
  m_tris = m_tin->Triangles();
  if (m_tin->TrisAdjToPts().empty())
    m_tin->BuildTrisAdjToPts();
  m_adjTris = m_tin->TrisAdjToPts();
  m_noDeletePts.clear();
  m_noDeletePts.insert(a_noDeletePts.begin(), a_noDeletePts.end());
  for (int i = 0; i < m_tin->NumPoints(); ++i)
  {
    if (m_noDeletePts.find(i) != m_noDeletePts.end())
      continue;
    if (m_adjTris[i].size() > 7)
    {
      m_ptIdx = i;
      m_removePts.insert(i);
      CalcPolygonFromPointIndex();
      CreateNewPoints();
    }
  }
  CreateNewTin();
  return m_outTin;
} // MePointConnectionFixerImpl::Fix
//------------------------------------------------------------------------------
/// \brief Creates a polygon from the triangles surrounding a point
//------------------------------------------------------------------------------
void MePointConnectionFixerImpl::CalcPolygonFromPointIndex()
{
  m_poly.clear();
  int edges[3][2] = {{1, 2}, {2, 0}, {0, 1}};
  int triPts[3] = {-1, -1, -1};
  VecInt adjTris = m_tin->TrisAdjToPts()[m_ptIdx];
  std::map<int, int> polyEdges;
  for (size_t i = 0; i < adjTris.size(); ++i)
  {
    int triIdx = adjTris[i] * 3;
    triPts[0] = m_tris[triIdx + 0];
    triPts[1] = m_tris[triIdx + 1];
    triPts[2] = m_tris[triIdx + 2];
    int locIdx = m_tin->LocalIndex(adjTris[i], m_ptIdx);
    int idx0 = edges[locIdx][0];
    int idx1 = edges[locIdx][1];
    polyEdges.insert(std::make_pair(triPts[idx0], triPts[idx1]));
  }
  XM_ENSURE_TRUE(!polyEdges.empty());
  
  auto it = polyEdges.begin();
  while (m_poly.size() < adjTris.size())
  {
    m_poly.push_back(it->first);
    it = polyEdges.find(it->second);
    if (it == polyEdges.end())
    {
      XM_ASSERT(true);
      m_poly.clear();
      return;
    }
  }
  // add the boundary as points that will not be deleted
  m_noDeletePts.insert(m_poly.begin(), m_poly.end());
  std::reverse(m_poly.begin(), m_poly.end());
} // MePointConnectionFixerImpl::CalcPolygonFromPointIndex
//------------------------------------------------------------------------------
/// \brief Remeshes the polygon around the point being removed.
//------------------------------------------------------------------------------
void MePointConnectionFixerImpl::CreateNewPoints()
{
  BSHP<MePolyMesher> pm = MePolyMesher::New();
  MeMultiPolyMesherIo mpInput;
  MePolyInput input;
  input.m_removeInternalFourTrianglePts = true;
  const VecPt3d& pts(m_tin->Points());
  for (size_t i = 0; i < m_poly.size(); ++i)
  {
    const Pt3d& p0(pts[m_poly[i]]);
    input.m_outPoly.push_back(p0);
  }
  // calc edge polygon edge lengths to set constant size function
  double sum_length = gmXyDistance(input.m_outPoly.front(), input.m_outPoly.back());
  for (size_t i = 0; i < m_poly.size() - 1; ++i)
  {
    sum_length += gmXyDistance(input.m_outPoly[i], input.m_outPoly[i + 1]);
  }
  double ave_length = sum_length / m_poly.size();
  input.m_constSizeFunction = 0.9 * ave_length;

  mpInput.m_polys.push_back(input);

  VecPt3d mpPts;
  VecInt mpTris, mpCells;
  pm->MeshIt(mpInput, 0, mpPts, mpTris, mpCells);
  if (mpPts.size() == input.m_outPoly.size())
  {// need to split some long edges because no internal points were created
    input.m_outPoly.push_back(input.m_outPoly[0]);
    double maxDist(0.0);
    VecDbl lengths;
    for (size_t i = 1; i < input.m_outPoly.size(); ++i)
    {
      const Pt3d& p0(input.m_outPoly[i - 1]);
      const Pt3d& p1(input.m_outPoly[i]);
      lengths.push_back(Mdist(p0.x, p0.y, p1.x, p1.y));
      if (lengths.back() > maxDist)
        maxDist = lengths.back();
    }
    int numNewPts(0);
    VecPt3d newPoly;
    double factor(0.75);
    while (numNewPts < 2)
    {
      numNewPts = 0;
      newPoly.clear();
      for (size_t i = 1; i < input.m_outPoly.size(); ++i)
      {
        newPoly.push_back(input.m_outPoly[i - 1]);
        if (factor * maxDist < lengths[i - 1])
        {
          numNewPts++;
          const Pt3d& p0(input.m_outPoly[i - 1]);
          const Pt3d& p1(input.m_outPoly[i]);
          Pt3d p = (p0 + p1) / 2;
          newPoly.push_back(p);
        }
      }
      factor -= 0.1;
    } 
    newPoly.pop_back(); // remove first point from the end
    input.m_outPoly = newPoly;
    mpInput.m_polys[0] = input;
    pm->MeshIt(mpInput, 0, mpPts, mpTris, mpCells);
  }
  // the new points should be the points in mpPts that are after the
  // input polygon
  for (size_t i = input.m_outPoly.size(); i < mpPts.size(); ++i)
  {
    m_newPts.push_back(mpPts[i]);
  }
} // MePointConnectionFixerImpl::CreateNewPoints
//------------------------------------------------------------------------------
/// \brief Creates a new TIN incorporating the new points
//------------------------------------------------------------------------------
void MePointConnectionFixerImpl::CreateNewTin()
{
  if (m_newPts.empty())
  {
    m_outTin = m_tin;
    return;
  }

  const VecPt3d& pts(m_tin->Points());
  BSHP<VecPt3d> newPts(new VecPt3d());
  newPts->reserve(pts.size() + m_newPts.size());
  for (size_t i = 0; i < pts.size(); ++i)
  {
    if (m_removePts.find((int)i) != m_removePts.end())
      continue;
    newPts->push_back(pts[i]);
  }
  for (size_t i = 0; i < m_newPts.size(); ++i)
  {
    newPts->push_back(m_newPts[i]);
  }

  m_outTin = TrTin::New();
  m_outTin->SetPoints(newPts);
  TrTriangulatorPoints client(*newPts.get(), m_outTin->Triangles(), &m_outTin->TrisAdjToPts());
  client.Triangulate();
} // MePointConnectionFixerImpl::CreateNewTin

} // namespace

////////////////////////////////////////////////////////////////////////////////
/// \class MePointConnectionFixer
/// \see MePointConnectionFixerImpl
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Create new MeBadQuadRemover.
/// \param[in] a_ugrid The UGrid to remove badly formed quads from.
/// \return The new MeBadQuadRemover.
//------------------------------------------------------------------------------
BSHP<MePointConnectionFixer> MePointConnectionFixer::New(BSHP<TrTin> a_tin)
{
  BSHP<MePointConnectionFixer> fixer(new MePointConnectionFixerImpl(a_tin));
  return fixer;
} // MePointConnectionFixer::New
//------------------------------------------------------------------------------
/// \brief Constructor.
//------------------------------------------------------------------------------
MePointConnectionFixer::MePointConnectionFixer()
{
} // MePointConnectionFixer::MePointConnectionFixer
//------------------------------------------------------------------------------
/// \brief Destructor.
//------------------------------------------------------------------------------
MePointConnectionFixer::~MePointConnectionFixer()
{
} // MePointConnectionFixer::~MePointConnectionFixer

} // namespace xms

#if CXX_TEST
////////////////////////////////////////////////////////////////////////////////
// UNIT TESTS
////////////////////////////////////////////////////////////////////////////////

#include <xmsmesher/meshing/detail/MePointConnectionFixer.t.h>
#include <fstream>
#include <xmscore/testing/TestTools.h>

//----- Namespace declaration --------------------------------------------------

using namespace xms;

//------------------------------------------------------------------------------
/// \brief utility to export TIN for debugging
/// \param[in] a_tin: The TIN to export
//------------------------------------------------------------------------------
void iExportTin(BSHP<TrTin> a_tin)
{
  std::string out_file = "C:/temp/out.tin";
  std::fstream os;
  os.open(out_file.c_str(), std::fstream::out);
  if (os.good())
  {
    a_tin->ExportTinFile(os);
 }
} // iExportTin
////////////////////////////////////////////////////////////////////////////////
/// \class MeBadQuadRemoverUnitTests
/// \brief Tests for MeBadQuadRemover class.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Test MePointConnectionFixer.
//------------------------------------------------------------------------------
void MePointConnectionFixerrUnitTests::testFixer()
{
  VecPt3d pts = {
    {26, 68, 0}, {21, 68, 0}, {28, 63, 0}, {31, 68, 0},
    {43, 75, 0}, {21, 56, 0}, {32, 57, 0}, {37, 63, 0},
    {36, 68, 0}, {41, 68, 0}, {46, 68, 0}, {51, 68, 0},
    {56, 67, 0}, {67, 76, 0}, {29, 47, 0}, {42, 54, 0},
    {45, 63, 0}, {53, 59, 0}, {61, 67, 0}, {66, 67, 0},
    {71, 67, 0}, {76, 68, 0}, {80, 69, 0}, {85, 70, 0},
    {90, 70, 0}, {48, 39, 0}, {65, 56, 0}, {75, 64, 0},
    {84, 63, 0}, {90, 54, 0}, {79, 44, 0}, {78, 57, 0}
 };
  BSHP<VecPt3d> ptsPtr(new VecPt3d(pts));
  BSHP<TrTin> tin = TrTin::New();
  tin->SetPoints(ptsPtr);
  TrTriangulatorPoints client(*ptsPtr.get(), tin->Triangles(), &tin->TrisAdjToPts());
  TS_ASSERT(client.Triangulate());
  BSHP<MePointConnectionFixer> fixer = MePointConnectionFixer::New(tin);
  TS_ASSERT(fixer);
  VecInt boundaryPoints;
  tin->GetBoundaryPoints(boundaryPoints);
  BSHP<TrTin> outTin = fixer->Fix(boundaryPoints);
  VecPt3d outPts = outTin->Points();
  VecPt3d basePts = pts;
  basePts.erase(basePts.begin() + 26);
  basePts.push_back(Pt3d(64.522, 49.966, 0));
  basePts.push_back(Pt3d(63.103, 59.578, 0));
  basePts.push_back(Pt3d(70.149, 59.490, 0));
  TS_ASSERT_DELTA_VECPT3D(basePts, outPts, 1e-2);
} // MePointConnectionFixerrUnitTests::testFixer
//------------------------------------------------------------------------------
/// \brief Test MePointConnectionFixer.
//------------------------------------------------------------------------------
void MePointConnectionFixerrUnitTests::testCase2()
{
  VecPt3d pts = {
    {180.6, 185.9, 0}, {177.4, 182.6, 0}, {174.2, 176.1, 0},
    {174.2, 167.1, 0}, {174.8, 159.6, 0}, {177.7, 154.8, 0},
    {185.8, 149.6, 0}, {192.3, 147.3, 0}, {198.1, 147.0, 0},
    {202.3, 148.3, 0}, {207.2, 152.5, 0}, {209.8, 158.7, 0},
    {210.8, 164.5, 0}, {210.8, 172.6, 0}, {210.8, 177.4, 0},
    {207.5, 183.6, 0}, {204.0, 185.9, 0}, {200.4, 187.2, 0},
    {196.8, 188.5, 0}, {190.7, 188.5, 0}, {193.6, 170.0, 0}
  };
  BSHP<VecPt3d> ptsPtr(new VecPt3d(pts));
  BSHP<TrTin> tin = TrTin::New();
  tin->SetPoints(ptsPtr);
  TrTriangulatorPoints client(*ptsPtr.get(), tin->Triangles(), &tin->TrisAdjToPts());
  TS_ASSERT(client.Triangulate());
  BSHP<MePointConnectionFixer> fixer = MePointConnectionFixer::New(tin);
  TS_ASSERT(fixer);
  VecInt boundaryPoints;
  tin->GetBoundaryPoints(boundaryPoints);
  BSHP<TrTin> outTin = fixer->Fix(boundaryPoints);
  VecPt3d outPts = outTin->Points();
  VecPt3d basePts = {
    {180.6, 185.9, 0}, {177.4, 182.6, 0}, {174.2, 176.1, 0}, {174.2, 167.1, 0}, {174.8, 159.6, 0},
    {177.7, 154.8, 0}, {185.8, 149.6, 0}, {192.3, 147.3, 0}, {198.1, 147.0, 0}, {202.3, 148.3, 0},
    {207.2, 152.5, 0}, {209.8, 158.7, 0}, {210.8, 164.5, 0}, {210.8, 172.6, 0}, {210.8, 177.4, 0},
    {207.5, 183.6, 0}, {204.0, 185.9, 0}, {200.4, 187.2, 0}, {196.8, 188.5, 0}, {190.7, 188.5, 0},
    {181.3, 165.0, 0}, {179.9, 171.2, 0}, {181.9, 176.5, 0}, {184.5, 182.3, 0}, {187.8, 178.3, 0},
    {191.9, 181.9, 0}, {196.7, 184.3, 0}, {202.1, 182.5, 0}, {204.4, 177.9, 0}, {205.9, 173.5, 0},
    {205.2, 168.4, 0}, {204.7, 162.7, 0}, {203.1, 157.3, 0}, {200.3, 152.7, 0}, {195.1, 151.9, 0},
    {190.4, 154.5, 0}, {185.3, 158.3, 0}, {191.8, 160.5, 0}, {187.8, 164.8, 0}, {185.6, 170.3, 0},
    {189.0, 174.0, 0}, {194.1, 176.5, 0}, {198.5, 178.9, 0}, {200.2, 173.1, 0}, {199.6, 167.0, 0},
    {198.6, 161.5, 0}, {196.4, 156.9, 0}, {195.1, 171.2, 0}, {194.2, 165.4, 0}, {190.8, 169.2, 0}
  };
  TS_ASSERT_DELTA_VECPT3D(basePts, outPts, 0.1);
} //  MePointConnectionFixerrUnitTests::testCase2


#endif // CXX_TEST
