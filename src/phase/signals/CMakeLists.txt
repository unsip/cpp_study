add_library(signals STATIC SignalMask.cpp)
target_include_directories(signals INTERFACE "${CMAKE_CURRENT_LIST_DIR}/")

add_library(phase::signals ALIAS signals)

