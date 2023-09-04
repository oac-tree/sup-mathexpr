add_executable(unit-tests)

target_sources(unit-tests
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/mathexpr_test.cpp
)

target_include_directories(unit-tests
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

find_package(GTest REQUIRED)

target_link_libraries(unit-tests PUBLIC GTest::GTest GTest::Main sup-mathexpr)

set_target_properties(unit-tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(unit-tests)
