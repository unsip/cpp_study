add_executable(
    WavingStuff
    main.cpp
    #OneLineRenderer.cpp
    NcurseOneLineRenderer.cpp
)

target_link_libraries(
    WavingStuff
    PUBLIC
      ${CURSES_NCURSES_LIBRARY}
)

add_subdirectory(tests)
