add_library(
    bestiary
    Bestiary.cpp
    Bestiary.hpp
    BestiaryFactory.cpp
    BestiaryFactory.hpp
    DescriptionOutput.cpp
    DescriptionOutput.hpp
    EventDispatcher.cpp
    EventDispatcher.hpp
    FreeOutput.cpp
    FreeOutput.hpp
    IBestiary.hpp
    IEventDispatcher.hpp
)
add_executable(
    battle
    main.cpp
)

target_link_libraries(
    battle
    PRIVATE
        bestiary
)

add_subdirectory(tests)
