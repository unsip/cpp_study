add_library(threading INTERFACE)
target_include_directories(threading INTERFACE "${CMAKE_CURRENT_LIST_DIR}/")

add_library(phase::threading ALIAS threading)
