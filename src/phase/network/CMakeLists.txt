add_library(network STATIC TcpSocket.cpp)
target_include_directories(network INTERFACE "${CMAKE_CURRENT_LIST_DIR}/")

add_library(phase::network ALIAS network)

