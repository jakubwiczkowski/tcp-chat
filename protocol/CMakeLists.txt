set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(protocol STATIC
        include/protocol/codec/codec.h
        src/codec/string_codec.cpp
        src/codec/uint8_codec.cpp
        src/codec/uint32_codec.cpp
        src/bytebuf/bytebuf.cpp
        include/protocol/bytebuf/bytebuf.h
        include/protocol/packet/packet.h
        include/protocol/packet/state.h
        include/protocol/packet/clientbound/chat/send_message.h
        src/packet/clientbound/chat/send_message.cpp
        include/protocol/packet/clientbound/config/acknowledge_name.h
        src/packet/clientbound/config/acknowledge_name.cpp
        include/protocol/packet/serverbound/chat/send_message.h
        src/packet/serverbound/chat/send_message.cpp
        include/protocol/packet/serverbound/config/set_name.h
        src/packet/serverbound/config/set_name.cpp
        include/protocol/codec/varint_codec.h)

target_include_directories(protocol PRIVATE src)
target_include_directories(protocol PUBLIC include)

set_target_properties(protocol PROPERTIES LINKER_LANGUAGE CXX)

if(BUILD_TESTING)
    add_executable(protocol_tests tests/test.cpp)
    target_link_libraries(protocol_tests PRIVATE Catch2::Catch2WithMain protocol)

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(Catch)
    catch_discover_tests(protocol_tests)
endif()
