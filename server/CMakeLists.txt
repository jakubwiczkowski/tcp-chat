set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server src/main.cpp
        src/server/server.cpp
        src/server/server.h)

target_link_libraries(server PRIVATE protocol)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})