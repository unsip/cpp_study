add_library(
    bestiary
    Bestiary.cpp
    Bestiary.hpp
    BestiaryFactory.cpp
    BestiaryFactory.hpp
    EventDispatcher.cpp
    EventDispatcher.hpp
    IBestiary.hpp
    IEventDispatcher.hpp
)
add_executable(
    battle
    main.cpp
)

target_link_libraries(
    battle
    PUBLIC
        bestiary
  )

add_subdirectory(tests)
