# Settings of CMake project

include(GNUInstallDirs)
include(CTest)
include(FindPackageMessage)

# Detecting CODAC environment
if(NOT COA_NO_CODAC)
  find_package(CODAC OPTIONAL_COMPONENTS site-packages Python MODULE QUIET)
endif()
if(CODAC_FOUND)
  set(CODAC_FOUND_MESSAGE "Building with CODAC")

  # Append CODAC_CMAKE_PREFIXES to cmake seard directories, this helps cmake find packages installed in the CODAC enviorenment
  list(APPEND CMAKE_PREFIX_PATH ${CODAC_CMAKE_PREFIXES})

  # If CODAC module provides python executable, override Python3_EXECUTABLE with it
  if(CODAC_Python_FOUND AND NOT Python3_EXECUTABLE)
    set(Python3_EXECUTABLE ${CODAC_PYTHON_EXECUTABLE})
  endif()

  # Check if operating inside a CODAC CICD system
  if(CODAC_CICD)
    string(APPEND CODAC_FOUND_MESSAGE " CICD environment")

    set(COA_BUILD_DOCUMENTATION ON)
  else()
    string(APPEND CODAC_FOUND_MESSAGE " environment")
  endif()

  find_package_message(
    CODAC_DETAILS
    "${CODAC_FOUND_MESSAGE}: ${CODAC_DIR} (version \"${CODAC_VERSION}\")"
    "[${CODAC_FOUND}][${CODAC_DIR}][${CODAC_CICD}][v${CODAC_VERSION}]"
  )
else()
  find_package_message(CODAC_DETAILS "Building without CODAC environment" "[${CODAC_FOUND}]")
endif()

get_filename_component(SUP_MATHEXPR_PROJECT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

# Build settings
if(COVERAGE)
  # On coverage builds  alsways skip building docs and build tests
  set(COA_BUILD_DOCUMENTATION OFF)
  set(COA_BUILD_TESTS ON)
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(LIBVERSION ${CMAKE_PROJECT_VERSION})
set(LIBSOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR})

# Directories
if (NOT DEFINED TEST_OUTPUT_DIRECTORY)
  set(TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_bin)
endif()

file(MAKE_DIRECTORY ${TEST_OUTPUT_DIRECTORY})
