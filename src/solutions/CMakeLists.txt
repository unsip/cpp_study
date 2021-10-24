add_executable(
    intervals_main
    intervals.cpp
    intervals_main.cpp
  )

add_executable(hello_ptr hello_ptr.cpp)
add_executable(ones_search ones_search.cpp)
add_executable(avg_mean avg_mean.cpp)

add_library(sll sll.cpp)
add_executable(sll_test sll_test.cpp)
target_link_libraries(sll_test PRIVATE sll)

add_subdirectory(tests)
