add_executable(
    WavingStuff
    main.cpp
  )

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
