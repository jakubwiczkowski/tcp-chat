set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server src/main.cpp
        src/server/server.cpp
        src/server/server.h
        src/server/client.h
        src/chat/chat_client.cpp
        src/chat/chat_client.h
        src/chat/chat_server.cpp
        src/chat/chat_server.h)

target_link_libraries(server PRIVATE common protocol)
target_include_directories(server PRIVATE src)