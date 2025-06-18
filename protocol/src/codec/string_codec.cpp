#include "protocol/codec/string_codec.h"

#include "protocol/codec/uint32_codec.h"
#include "protocol/codec/varint_codec.h"

std::string string_codec::decode(bytebuf& buffer) const {
    // strings are prepended with unsigned 32bit varint length

    if (buffer.remaining() < 1) {
        throw std::out_of_range("Buffer size not enough (to read length)");
    }

    uint32_t length = UVARINT32_CODEC.decode(buffer);
    if (buffer.remaining() < length) {
        throw std::out_of_range("Buffer size not enough (no space in buffer)");
    }

    std::string result;
    for (size_t i = 0; i < length; i++)
        result.push_back(static_cast<char>(buffer.read()));

    return result;
}

void string_codec::encode(bytebuf& buffer, std::string data) const {
    bytebuf length_buffer;
    UVARINT32_CODEC.encode(length_buffer, data.length());

    buffer.write(length_buffer);

    for (const char c : data) buffer.write(c);
}
