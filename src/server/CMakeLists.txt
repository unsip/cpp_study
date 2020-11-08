find_package(Threads REQUIRED)

# TODO: Add doxygen.
#add_library(
#    tcp_socket
#    TcpSocket.cpp
#    TcpSocket.hpp
#    utils/SignalMask.cpp
#    )
#
#target_compile_options(
#    tcp_socket
#    INTERFACE
#        "-pthread"
#    )

add_executable(tcp_srv Server.cpp)

target_link_libraries(
    tcp_srv
    PRIVATE
        phase
        Threads::Threads
)

target_compile_options(tcp_srv PUBLIC "-pthread")

add_executable(
    tcp_client
    Client.cpp
)

target_link_libraries(
    tcp_client
    PRIVATE
        phase::network
)
