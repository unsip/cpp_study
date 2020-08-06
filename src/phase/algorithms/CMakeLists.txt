add_library(phase INTERFACE)
set_target_properties(
    phase
PROPERTIES
INTERFACE_INCLUDE_DIRECTORIES: "${CMAKE_CURRENT_LIST_DIR}/"
)
