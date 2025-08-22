if(NOT COA_PARASOFT_INTEGRATION)
  return()
endif()

include(COASetupParasoft)

# Add parasoft project for sup-mathexpr-target
# This generates the build time information needed by Parasoft for analysis
cpptest_add_executable(sup-mathexpr-cpptest
  CPPTEST_PROJECT_LOC ${CMAKE_CURRENT_BINARY_DIR}
  CPPTEST_PROJECT_FOLDERS sup-mathexpr=${CMAKE_CURRENT_SOURCE_DIR}/src/lib/sup/mathexpr
  TARGETS sup-mathexpr
)
get_target_property(SUP_MATHEXPR_INCLUDE_DIRECTORIES sup-mathexpr INCLUDE_DIRECTORIES)
target_include_directories(sup-mathexpr-cpptest PUBLIC ${SUP_MATHEXPR_INCLUDE_DIRECTORIES})
