add_library(
    signals
    SignalMask.cpp
    SignalMask.hpp
  )

target_include_directories(
    signals
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
  )

add_library(phase::signals ALIAS signals)
