# -----------------------------------------------------------------------------
# Installation
# Credits to https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right,
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/Exporting-and-Importing-Targets
# -----------------------------------------------------------------------------

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/sup-mathexpr)

# exporting targets to a script and installing it
install(EXPORT sup-mathexpr-targets FILE sup-mathexpr-targets.cmake NAMESPACE sup-mathexpr:: DESTINATION ${INSTALL_CONFIGDIR})

# -----------------------------------------------------------------------------
# Exporting
# -----------------------------------------------------------------------------

# Add all targets to the build-tree export set
export(TARGETS sup-mathexpr NAMESPACE sup-mathexpr:: FILE "${PROJECT_BINARY_DIR}/sup-mathexpr-targets.cmake")

# Export the package for use from the build-tree (goes to $HOME/.cmake)
if(COA_EXPORT_BUILD_TREE)
  set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
  export(PACKAGE sup-mathexpr)
endif()

# -----------------------------------------------------------------------------
# Creating and installing sup-mathexpr-config.cmake
# -----------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

# to use in the build tree
configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/config/sup-mathexpr-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/sup-mathexpr-config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# to use in install tree
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sup-mathexpr-config.cmake DESTINATION ${INSTALL_CONFIGDIR})

# -----------------------------------------------------------------------------
# Create and install sup-mathexpr-config-version.cmake file
# -----------------------------------------------------------------------------

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/sup-mathexpr-config-version.cmake VERSION
    ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sup-mathexpr-config-version.cmake DESTINATION ${INSTALL_CONFIGDIR})
