#pragma once
#ifdef CXX_TEST
//------------------------------------------------------------------------------
/// \file
/// \ingroup meshing
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

// 3. Standard Library Headers

// 4. External Library Headers
#include <cxxtest/TestSuite.h>

// 5. Shared Headers

// 6. Non-shared Headers

//----- Namespace declaration --------------------------------------------------

// namespace xms {

class MeMultiPolyTo2dmUnitTests : public CxxTest::TestSuite
{
public:
  void testCreateClass();
  void testCase4();
};

class MeMultiPolyTo2dmIntermediateTests : public CxxTest::TestSuite
{
public:
  void testCase2();
  void testCase100();
  void testCase101();
  void testCase102();
  void testCase103();
  void testCasePaveGeo();
  void testCasePaveSanDiego();
  void testCasePaveSanDiego_SpringRelaxation();
  void testCasePatch6();
  void testCasePaveConstSizeTransition();
  void testSeedPoints();
  void testSeedPoints_PolygonWithHole();
  void testbug11299();
  void testInternalFeaturesCase0();
  void testInternalFeaturesCase1();
  void testInternalFeaturesCase2();
  void testInternalFeaturesCase3();
  void testbug11646();
  void testbug12780();
  void testbug13701();
  void testbug13701_a();
  void test_srp_seed_pts();
  void testbug14016();
  void testbug14016_no_interior();
  void testbug14901();
  void testbug15183();
  void testbug15186();
  void testbug15246();
  void testPatch7();
  void testRefine();
  void testRefine_no_interior();
};

//} // namespace xms
#endif
