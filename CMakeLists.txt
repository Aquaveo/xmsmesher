# This is a generated file.
# To regenerate file, use generators in the aquaveo/dev_tools repository.
# Any changes needed to be made to the file should be made in the template file.

cmake_minimum_required(VERSION 3.11.0)
cmake_policy(SET CMP0015 NEW) # Link Directory Pathing
cmake_policy(SET CMP0091 NEW)
set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_CXX_STANDARD 17)

project(xmsmesherlib C CXX)

# Cache variables
set(BUILD_TESTING NO CACHE BOOL "Enable/Disable testing")
set(IS_CONDA_BUILD NO CACHE BOOL "Set this if you want to make a conda package.")
set(IS_EMSCRIPTEN_BUILD NO CACHE BOOL "Set this to build with emscripten using cmake.")
set(PYTHON_TARGET_VERSION 3.10 CACHE STRING "Version of python to link to for python wrapping.")
set(CONDA_PREFIX "" CACHE PATH "Path to the conda environment used to build.")
set(IS_PYTHON_BUILD NO CACHE BOOL "Set this if you want to build the python bindings.")
set(XMS_TEST_PATH ${PROJECT_SOURCE_DIR}/test_files/ CACHE PATH "Path to test files for testing")
set(XMS_VERSION "99.99.99" CACHE STRING "Library Version")

# Static library sources
set(xmsmesher_sources
    xmsmesher/meshing/MeMeshUtils.cpp
    xmsmesher/meshing/MeMultiPolyTo2dm.cpp
    xmsmesher/meshing/MeMultiPolyMesher.cpp
    xmsmesher/meshing/detail/MeBadQuadRemover.cpp
    xmsmesher/meshing/detail/MeIntersectPolys.cpp
    xmsmesher/meshing/detail/MePointConnectionFixer.cpp
    xmsmesher/meshing/detail/MePolyPatcher.cpp
    xmsmesher/meshing/detail/MePolyOffsetter.cpp
    xmsmesher/meshing/detail/MePolyPaverToMeshPts.cpp
    xmsmesher/meshing/detail/MePolyCleaner.cpp
    xmsmesher/meshing/detail/MePolyPts.cpp
    xmsmesher/meshing/detail/MePolyRedistributePtsCurvature.cpp
    xmsmesher/meshing/detail/MeQuadBlossom.cpp
    xmsmesher/meshing/detail/MeRefinePtsToPolys.cpp
    xmsmesher/meshing/detail/MeRelaxer.cpp
    xmsmesher/meshing/detail/MeWeightMatcher.cpp
    xmsmesher/meshing/MePolyMesher.cpp
    xmsmesher/meshing/MePolyRedistributePts.cpp
    xmsmesher/tutorial/TutMeshing.cpp
)

# Static library headers
set(xmsmesher_headers
    xmsmesher/meshing/MeMeshUtils.h
    xmsmesher/meshing/MePolyMesher.h
    xmsmesher/meshing/MeMultiPolyMesher.h
    xmsmesher/meshing/MeMultiPolyMesherIo.h
    xmsmesher/meshing/MeMultiPolyTo2dm.h
    xmsmesher/meshing/MePolyRedistributePts.h
    xmsmesher/meshing/detail/MeBadQuadRemover.h
    xmsmesher/meshing/detail/MePolyCleaner.h
    xmsmesher/meshing/detail/MePolyOffsetter.h
    xmsmesher/meshing/detail/MePolyPts.h
    xmsmesher/meshing/detail/MePolyPatcher.h
    xmsmesher/meshing/detail/MeIntersectPolys.h
    xmsmesher/meshing/detail/MePointConnectionFixer.h
    xmsmesher/meshing/detail/MePolyPaverToMeshPts.h
    xmsmesher/meshing/detail/MePolyRedistributePtsCurvature.h
    xmsmesher/meshing/detail/MeQuadBlossom.h
    xmsmesher/meshing/detail/MeRefinePtsToPolys.h
    xmsmesher/meshing/detail/MeRelaxer.h
    xmsmesher/meshing/detail/MeWeightMatcher.h
)

if (APPLE OR WIN32 OR IS_EMSCRIPTEN_BUILD)
    # Target C++17 on MacOS and Windows. If building with version of Visual Studio without
    # support, has no effect.
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 11)
endif ()

if (APPLE)
    set(CMAKE_POSITION_INDEPENDENT_CODE False)
else ()
    set(CMAKE_POSITION_INDEPENDENT_CODE True)
endif ()

if (IS_PYTHON_BUILD AND BUILD_TESTING)
    message(FATAL_ERROR "Cannot build python module when testing is enabled")
endif ()

add_definitions(-DXMS_VERSION=\"${XMS_VERSION}\")

if (IS_CONDA_BUILD)
    include(${CMAKE_CURRENT_LIST_DIR}/condabuildinfo.cmake)
elseif (IS_EMSCRIPTEN_BUILD)
    include(${CMAKE_CURRENT_LIST_DIR}/emscriptenbuildinfo.cmake)
else () # If we are not using conda or emscripten, we are using conan
    # Conan 2 setup
    find_package(xmscore REQUIRED)
    find_package(xmsgrid REQUIRED)
    find_package(xmsinterp REQUIRED)
    find_package(Boost REQUIRED)
    find_package(ZLIB REQUIRED)
    list(APPEND EXT_INCLUDE_DIRS ${xmscore_INCLUDE_DIRS})
    list(APPEND EXT_INCLUDE_DIRS ${xmsgrid_INCLUDE_DIRS})
    list(APPEND EXT_INCLUDE_DIRS ${xmsinterp_INCLUDE_DIRS})
    list(APPEND EXT_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
    list(APPEND EXT_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
    list(APPEND EXT_LIB_DIRS ${xmscore_LIBRARY_DIRS})
    list(APPEND EXT_LIB_DIRS ${xmsgrid_LIBRARY_DIRS})
    list(APPEND EXT_LIB_DIRS ${xmsinterp_LIBRARY_DIRS})
    list(APPEND EXT_LIB_DIRS ${Boost_LIBRARY_DIRS})
    list(APPEND EXT_LIB_DIRS ${ZLIB_LIBRARY_DIRS})
    list(APPEND EXT_LIBS ${xmscore_LIBRARIES})
    list(APPEND EXT_LIBS ${xmsgrid_LIBRARIES})
    list(APPEND EXT_LIBS ${xmsinterp_LIBRARIES})
    list(APPEND EXT_LIBS ${Boost_LIBRARIES})
    list(APPEND EXT_LIBS ${ZLIB_LIBRARIES})
endif ()

if (WIN32)
    string(COMPARE EQUAL "${CONAN_SETTINGS_COMPILER_RUNTIME}" "MT" USES_MT)
    if (NOT USES_MT)
        string(COMPARE EQUAL "${CONAN_SETTINGS_COMPILER_RUNTIME}" "MTd" USES_MT)
    endif ()

    if (USE_TYPEDEF_WCHAR_T)
        add_definitions(/Zc:wchar_t-)  # Treat wchar_t as a typedef
    endif ()

    add_definitions(/D BOOST_ALL_NO_LIB)

    if (USES_MT)
        set(CompilerFlags
                CMAKE_CXX_FLAGS
                CMAKE_CXX_FLAGS_DEBUG
                CMAKE_CXX_FLAGS_RELEASE
                CMAKE_C_FLAGS
                CMAKE_C_FLAGS_DEBUG
                CMAKE_C_FLAGS_RELEASE
        )
        foreach (CompilerFlag ${CompilerFlags})
            string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach ()
    endif ()
endif ()

if (IS_PYTHON_BUILD)

    find_package(pybind11 REQUIRED)
    list(APPEND EXT_INCLUDE_DIRS
            ${pybind11_INCLUDE_DIR}
    )

    find_package(PythonLibsNew ${PYTHON_TARGET_VERSION} EXACT REQUIRED)

    # Have to add this after conda because it doesn't get the path for pybind if we don't.
    list(APPEND EXT_INCLUDE_DIRS
            ${PYTHON_INCLUDE_DIRS}
    )
endif ()

if (BUILD_TESTING)
    message(STATUS "Adding test dependencies...")
    add_definitions(-DXMS_TEST_PATH="${XMS_TEST_PATH}/")
    add_definitions(-DCXX_TEST -DCXXTEST4)

    list(APPEND xmsmesher_headers
         xmsmesher/meshing/MeMeshUtils.t.h
         xmsmesher/meshing/MeMultiPolyTo2dm.t.h
         xmsmesher/meshing/MePolyMesher.t.h
         xmsmesher/meshing/MeMultiPolyMesher.t.h
         xmsmesher/meshing/MePolyRedistributePts.t.h
         xmsmesher/meshing/detail/MeBadQuadRemover.t.h
         xmsmesher/meshing/detail/MePolyPaverToMeshPts.t.h
         xmsmesher/meshing/detail/MeIntersectPolys.t.h
         xmsmesher/meshing/detail/MePointConnectionFixer.t.h
         xmsmesher/meshing/detail/MePolyPatcher.t.h
         xmsmesher/meshing/detail/MePolyOffsetter.t.h
         xmsmesher/meshing/detail/MePolyCleaner.t.h
         xmsmesher/meshing/detail/MePolyRedistributePtsCurvature.t.h
         xmsmesher/meshing/detail/MeQuadBlossom.t.h
         xmsmesher/meshing/detail/MeRefinePtsToPolys.t.h
         xmsmesher/meshing/detail/MeRelaxer.t.h
         xmsmesher/meshing/detail/MeWeightMatcher.t.h
         xmsmesher/testing/TestTools.cpp
         xmsmesher/testing/TestTools.h
         xmsmesher/tutorial/TutMeshing.t.h
    )

    find_package(cxxtest REQUIRED)
    list(APPEND EXT_INCLUDE_DIRS ${cxxtest_INCLUDE_DIRS})
    list(APPEND EXT_LIB_DIRS ${cxxtest_LIBRARY_DIRS})
    list(APPEND EXT_LIBS ${cxxtest_LIBRARIES})

    if (cxxtest_FOUND)
        include("FindCxxTest")
        include_directories(${CXXTEST_INCLUDE_DIR})
        enable_testing()

        set(cxxtest_TESTGEN_ARGS --xunit-printer --have-eh)
        file(GLOB_RECURSE test_headers ${CMAKE_CURRENT_LIST_DIR}/xmsmesher/*.t.h)
        message(STATUS "Prefix Path: ${CMAKE_PREFIX_PATH}")
        message(STATUS "Module Path: ${CMAKE_MODULE_PATH}")
        CXXTEST_ADD_TEST(
                runner runner.cpp ${test_headers}
        )
        target_link_libraries(runner ${PROJECT_NAME})

        if (WIN32)
            set(RUNNERNAME "runner.exe")
        else ()
            set(RUNNERNAME "runner")
        endif ()
    endif ()
endif ()


message(STATUS "External Include Dirs: ${EXT_INCLUDE_DIRS}")
message(STATUS "External Lib Dirs: ${EXT_LIB_DIRS}")
message(STATUS "External Libs: ${EXT_LIBS}")


include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${EXT_INCLUDE_DIRS})
link_directories(${EXT_LIB_DIRS})


message("External Include Dirs: ${EXT_INCLUDE_DIRS}")
message("External Lib Dirs: ${EXT_LIB_DIRS}")
message("Extneral Libs: ${EXT_LIBS}")

# Static library
add_library(${PROJECT_NAME} STATIC
        ${xmsmesher_sources} ${xmsmesher_headers}
)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}
        ${EXT_LIBS}
)
target_link_libraries(${PROJECT_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
)
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} rt)
endif ()


# Pybind11
if (IS_PYTHON_BUILD)
    # Pybind11 sources
    set(xmsmesher_py
        xmsmesher/python/xmsmesher_py.cpp
        xmsmesher/python/meshing/meshing_py.cpp
        xmsmesher/python/meshing/MeMeshUtils_py.cpp
        xmsmesher/python/meshing/MeMultiPolyMesherIo_py.cpp
        xmsmesher/python/meshing/MePolyRedistributePts_py.cpp
    )

    set(xmsmesher_py_headers
        xmsmesher/python/meshing/meshing_py.h
    )

    message(STATUS "PYTHON_INCLUDE: ${PYTHON_INCLUDE_DIRS}")
    message(STATUS "PYTHON_LIBS: ${PYTHON_LIBRARIES}")

    pybind11_add_module(_xmsmesher
            ${xmsmesher_py} ${xmsmesher_py_headers}
    )
    target_include_directories(_xmsmesher
            PRIVATE
            ${EXT_INCLUDE_DIRS}
    )
    target_link_libraries(_xmsmesher
            PRIVATE
            ${EXT_LIBS}
            ${PROJECT_NAME}
    )
    set_target_properties(_xmsmesher PROPERTIES
            LINKER_LANGUAGE CXX
    )

    install(
            TARGETS _xmsmesher
            ARCHIVE DESTINATION "_package/xms/mesher"
            LIBRARY DESTINATION "_package/xms/mesher"
    )

    install(DIRECTORY "_package/" DESTINATION "_package"
            FILES_MATCHING PATTERN "*.py"
            PATTERN "_package/tests" EXCLUDE)
endif ()


# Install recipe
install(
        TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION "lib"
        LIBRARY DESTINATION "lib"
)
foreach (header IN LISTS xmsmesher_headers xmsmesher_py_headers)
    get_filename_component(subdir "${header}" DIRECTORY)
    install(
            FILES "${header}"
            DESTINATION "include/${subdir}"
    )
endforeach ()
