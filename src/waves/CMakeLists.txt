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

target_compile_options(
    WavingStuff
    PRIVATE
        ${CURSES_CFLAGS}
    )

target_include_directories(
    WavingStuff
    PRIVATE
        ${CURSES_INCLUDE_DIRS}
    )

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
