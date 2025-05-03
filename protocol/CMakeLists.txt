set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

add_library(protocol STATIC
        src/codec/codec.h
        src/codec/uint32_codec.h
        src/codec/string_codec.h
        src/bytebuf/bytebuf.cpp
        src/bytebuf/bytebuf.h
        src/packet/packet.h
        src/packet/serverbound/config/set_name.h
        src/packet/clientbound/config/acknowledge_name.h
        src/codec/uint8_codec.h
        src/packet/state.h
        src/packet/serverbound/chat/send_message.h
        src/packet/clientbound/chat/send_message.h)

target_include_directories(protocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(protocol PROPERTIES LINKER_LANGUAGE CXX)
