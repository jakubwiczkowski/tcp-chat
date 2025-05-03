cmake_minimum_required(VERSION 3.30)
project(tcp_chat)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(common)
add_subdirectory(protocol)
add_subdirectory(client)
add_subdirectory(server)
