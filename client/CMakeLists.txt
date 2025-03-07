set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(client src/main.cpp)

target_link_libraries(client PRIVATE protocol)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})