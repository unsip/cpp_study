add_subdirectory(algorithms)
add_subdirectory(network)
add_subdirectory(signals)
add_subdirectory(tests)
add_subdirectory(threading)

add_library(phase INTERFACE)
target_link_libraries(phase INTERFACE
    phase::algorithms
    phase::network
    phase::signals
    phase::threading
)
