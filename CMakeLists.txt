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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

include(CTest)
if(BUILD_TESTING)
    find_package(GTest REQUIRED)
endif()

add_subdirectory(src)
