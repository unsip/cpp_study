add_subdirectory(algorithms)
add_subdirectory(tests)

add_library(phase INTERFACE)
target_link_libraries(phase INTERFACE
    phase::algorithms
)
