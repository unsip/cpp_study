find_package(Threads REQUIRED)

# TODO: Add doxygen.
add_library(
    tcp_socket
    TcpSocket.cpp
    TcpSocket.hpp
    utils/SignalMask.cpp
    )

target_compile_options(
    tcp_socket
    INTERFACE
        "-pthread"
    )

add_executable(
    tcp_srv
    Server.cpp
    )

target_link_libraries(
    tcp_srv
    PRIVATE
        tcp_socket
        Threads::Threads
    )

add_executable(
    tcp_client
    Client.cpp
    )

target_link_libraries(
    tcp_client
    PRIVATE
        tcp_socket
    )
