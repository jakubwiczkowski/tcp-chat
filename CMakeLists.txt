cmake_minimum_required(VERSION 3.30)
project(tcp_chat)
enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(common)
add_subdirectory(protocol)
add_subdirectory(client)
add_subdirectory(server)
