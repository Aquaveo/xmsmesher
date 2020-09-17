//------------------------------------------------------------------------------
/// \file
/// \brief Functions for running unit or intermediate tests
/// \ingroup testing
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsmesher/testing/TestTools.h>

// 3. Standard library headers

// 4. External library headers
#include <boost/predef.h>
#include <boost/version.hpp>

// 5. Shared code headers
#include <xmscore/testing/TestTools.h>

// 6. Non-shared code headers

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

namespace
{
//------------------------------------------------------------------------------
/// \brief See if file exists for testing.
/// \param[in] a_filePath: Path to check for existing file.
/// \return true if found.
//------------------------------------------------------------------------------
bool iFindTestFile(const std::string& a_filePath)
{
  FILE* fp;
  bool found;
#if BOOST_OS_WINDOWS
  if (fopen_s(&fp, a_filePath.c_str(), "r") || fp == nullptr)
    found = false;
  else
    found = true;
#else
  fp = fopen(a_filePath.c_str(), "r");
  found = fp != NULL;
#endif
  if (fp)
    fclose(fp);
  return found;
} // iFindTestFile
} // namespace {
//------------------------------------------------------------------------------
/// \brief Get testing base and output file using the boost Geometry library.
///        Checks for appended "BoostGeom174" for a boost library greater than
///        1.73.0 builds. For example with Mac OS 64-bit build looks for
///        test_baseBoostGeom174.vtu then if not found test_base64.vtu then
///        test_base.vtu.
/// \param[in] a_path: Path to the file.
/// \param[in] a_fileBase: File name without extension.
/// \param[in] a_extension: File extension.
/// \param[out] a_baseFilePath: Path to base file.
/// \param[out] a_outFilePath: Path to output file.
//------------------------------------------------------------------------------
void ttGetTestFilePathsBoostGeom(const std::string& a_path,
                                 const std::string& a_fileBase,
                                 const std::string& a_extension,
                                 std::string& a_baseFilePath,
                                 std::string& a_outFilePath)
{
  bool foundBase = false;
#if BOOST_VERSION > 107300
  // see if there is a different base file for mac os x
  if (!foundBase)
  {
    a_baseFilePath = a_path + a_fileBase + "_baseBoostGeom174" + a_extension;
    foundBase = iFindTestFile(a_baseFilePath);
  }
#endif

  if (!foundBase)
  {
    ttGetTestFilePaths(a_path, a_fileBase, a_extension, a_baseFilePath, a_outFilePath);
  }
  else
  {
    a_outFilePath = a_path + a_fileBase + "_out" + a_extension;
  }
} // ttGetTestFilePaths
} // namespace xms
