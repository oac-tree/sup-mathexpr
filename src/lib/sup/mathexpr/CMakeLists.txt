add_library(sup-mathexpr SHARED)
add_library(sup-mathexpr::sup-mathexpr ALIAS sup-mathexpr) # alias for build-tree usage

target_compile_features(sup-mathexpr PRIVATE cxx_std_11)

set_target_properties(sup-mathexpr PROPERTIES
  VERSION ${LIBVERSION}
  SOVERSION ${LIBSOVERSION}
  OUTPUT_NAME sup-mathexpr
)

target_sources(sup-mathexpr
  PRIVATE
    exceptions.cpp
    expression_context.cpp
)

target_include_directories(sup-mathexpr PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

install(TARGETS sup-mathexpr EXPORT sup-mathexpr-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(sup-mathexpr PROPERTIES EXPORT_NAME sup-mathexpr)

install(FILES
  exceptions.h
  expression_context.h
  i_variable_store.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/mathexpr
)
