add_library(
    tcp_socket
    TcpSocket.cpp
    TcpSocket.hpp
    )

add_executable(
    tcp_srv
    Server.cpp
    )

target_link_libraries(
    tcp_srv
    PRIVATE
        tcp_socket
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
