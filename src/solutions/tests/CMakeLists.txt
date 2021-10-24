add_executable(
    intervals_test
    ../intervals.cpp
    intervals_test.cpp
  )

target_link_libraries(
    intervals_test
    PRIVATE
        GTest::GTest
        GTest::Main
  )

gtest_discover_tests(intervals_test)
