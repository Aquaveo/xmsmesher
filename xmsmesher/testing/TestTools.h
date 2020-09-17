#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup testing
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard library headers
#include <sstream> // for sstream
#include <vector>

// 4. External library headers
#ifdef CXX_TEST
#include <cxxtest/TestSuite.h>
#else
#error "Should only be included in builds with testing enabled."
#endif // ifdef CXX_TEST

// 5. Shared code headers
#include <xmscore/points/ptsfwd.h>
#include <xmscore/stl/utility.h>

namespace xms
{

//----- Function prototypes ----------------------------------------------------

void ttGetTestFilePathsBoostGeom(const std::string& a_path,
                                 const std::string& a_fileBase,
                                 const std::string& a_extension,
                                 std::string& a_baseFilePath,
                                 std::string& a_outFilePath);

} // namespace xms
