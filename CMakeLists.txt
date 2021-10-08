# ALERT Always use the latest available release!
cmake_minimum_required(VERSION 3.14)

project(
    cpp_study
    DESCRIPTION "Just trying out things"
    HOMEPAGE_URL "https://github.com/unsip/cpp_study"
    VERSION 0.0.1
    LANGUAGES CXX C
  )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
add_compile_options(
    # TODO Wrap into genex if other than GCC/Clang compilers needed
    -Wall
    -Wextra
    -Wpedantic
)

find_package(Curses REQUIRED)
find_package(GTest)

enable_testing()

add_subdirectory(src)
