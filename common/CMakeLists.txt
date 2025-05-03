set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(common STATIC
        src/sync/sync.h
        src/logger/logger.h
        src/logger/logger.cpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)
