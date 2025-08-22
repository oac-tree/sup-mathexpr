set(unit-tests sup-mathexpr-unit-tests)

add_executable(${unit-tests})

set_target_properties(${unit-tests} PROPERTIES OUTPUT_NAME "unit-tests")
set_target_properties(${unit-tests} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

target_sources(${unit-tests}
  PRIVATE
    exception_tests.cpp
    mathexpr_tests.cpp
)

target_include_directories(${unit-tests}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

find_package(GTest REQUIRED)

include(GoogleTest)

target_link_libraries(${unit-tests} PUBLIC GTest::GTest GTest::Main sup-mathexpr)

gtest_discover_tests(${unit-tests})
