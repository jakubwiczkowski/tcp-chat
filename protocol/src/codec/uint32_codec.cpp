#include "protocol/codec/uint32_codec.h"

uint32_t uint32_codec::decode(bytebuf& buffer) const {
    if (buffer.remaining() < 4) {
        throw std::out_of_range("Buffer size not enough");
    }

    return (buffer.read() << 24) | (buffer.read() << 16) |
           (buffer.read() << 8) | buffer.read();
}

void uint32_codec::encode(bytebuf& buffer, uint32_t data) const {
    buffer.write(data >> 24);
    buffer.write(data >> 16);
    buffer.write(data >> 8);
    buffer.write(data);
}
