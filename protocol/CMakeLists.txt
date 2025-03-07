set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(protocol STATIC src/test.cpp)

target_include_directories(protocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})