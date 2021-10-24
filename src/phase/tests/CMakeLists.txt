add_executable(
    AlgorithmsTests
    HeapSortTests.cpp
    MergeSortTests.cpp
    QuickSortTests.cpp
  )

target_link_libraries(
    AlgorithmsTests
    PRIVATE
        phase::algorithms
        GTest::GTest
        GTest::Main
  )

gtest_discover_tests(AlgorithmsTests)
