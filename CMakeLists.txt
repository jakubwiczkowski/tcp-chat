cmake_minimum_required(VERSION 3.30)
project(tcp_chat)
enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.1
)

FetchContent_MakeAvailable(Catch2)

add_subdirectory(common)
add_subdirectory(protocol)
add_subdirectory(client)
add_subdirectory(server)
