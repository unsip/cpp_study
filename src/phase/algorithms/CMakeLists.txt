add_library(algorithms INTERFACE)
target_include_directories(algorithms INTERFACE "${CMAKE_CURRENT_LIST_DIR}/")

add_library(phase::algorithms ALIAS algorithms)

