set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server src/main.cpp
        src/server/server.cpp
        src/server/server.h
        src/server/client.cpp
        src/server/client.h
        src/sync/sync.h
        src/chat/chat_client.cpp
        src/chat/chat_client.h)

target_link_libraries(server PRIVATE protocol)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})