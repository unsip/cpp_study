# TODO: Add doxygen.
#add_library(
#    tcp_socket
#    TcpSocket.cpp
#    TcpSocket.hpp
#    utils/SignalMask.cpp
#    )
#

add_executable(
    tcp_srv
    Server.cpp
  )

target_link_libraries(
    tcp_srv
    PRIVATE
        phase
        Threads::Threads
  )

add_executable(
    tcp_client
    Client.cpp
  )

target_link_libraries(
    tcp_client
    PRIVATE
        phase::network
  )
