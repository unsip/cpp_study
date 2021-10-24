add_executable(
    WavingStuff
    main.cpp
    OneLineRenderer.cpp
    NcurseOneLineRenderer.cpp
  )

target_link_libraries(
    WavingStuff
    PRIVATE
      ${CURSES_NCURSES_LIBRARY}
      ${CURSES_EXTRA_LIBRARY}
  )

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
