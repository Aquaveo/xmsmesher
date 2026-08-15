//------------------------------------------------------------------------------
/// \file
/// \ingroup meshing
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------
#pragma once
#ifdef CXX_TEST

//----- Included files ---------------------------------------------------------
// 3. Standard Library Headers

// 4. External Library Headers
#include <cxxtest/TestSuite.h>

// 5. Shared Headers

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
class InterpRasterSizeFunctionUnitTests : public CxxTest::TestSuite
{
public:
  void testNewValidConstruction();
  void testNewThrowsOnNonPositiveNx();
  void testNewThrowsOnNonPositiveNy();
  void testNewThrowsOnZeroDx();
  void testNewThrowsOnZeroDy();
  void testNewThrowsOnMismatchedValuesSize();
  void testGetPtsAndTrisBoundingGeometry();
  void testInterpToPtOnGridCorner();
  void testInterpToPtBilinearInterior();
  void testInterpToPtOutsideClampsToNearestCell();
  void testInterpToPts();
  void testSetTruncDefaultsToOff();
  void testSetTruncClampsInterpolatedValue();
  void testSetScalarsFromPointer();
  void testSetScalarsFromSharedPtr();
  void testPtAndTriActivityAreEmpty();
  void testSetPtsTrisIsNoOp();
};
//----- Function prototypes ----------------------------------------------------

#endif
