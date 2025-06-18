set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(common STATIC
        include/common/sync/sync.h
        include/common/logger/logger.h
        src/logger/logger.cpp
)

target_include_directories(common PRIVATE src)
target_include_directories(common PUBLIC include)

set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)
