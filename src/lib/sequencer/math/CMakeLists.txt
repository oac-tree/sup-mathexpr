add_library(sequencer-math SHARED)

target_compile_features(sequencer-math PRIVATE cxx_std_11)

set_target_properties(sequencer-math PROPERTIES
    EXPORT_NAME sequencer-math
    SOVERSION ${SUP_SEQUENCER_PLUGIN_MATH_SOVERSION}
    VERSION ${SUP_SEQUENCER_PLUGIN_MATH_BUILDVERSION}
    LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_RUNTIME_DIRECTORY}
)

target_sources(sequencer-math
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/math_instruction.cpp
        ${CMAKE_CURRENT_LIST_DIR}/expressioncontext.cpp
        ${CMAKE_CURRENT_LIST_DIR}/expressionevaluator.cpp
)

target_include_directories(sequencer-math PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

target_link_libraries(sequencer-math PUBLIC sequencer::sequencer)

install(TARGETS sequencer-math DESTINATION ${PLUGIN_PATH})
