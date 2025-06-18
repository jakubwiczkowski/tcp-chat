#include "protocol/codec/uint8_codec.h"

uint8_t uint8_codec::decode(bytebuf& buffer) const {
    if (buffer.remaining() < 1) {
        throw std::out_of_range("Buffer size not enough");
    }

    return buffer.read();
}

void uint8_codec::encode(bytebuf& buffer, uint8_t data) const {
    buffer.write(data);
}
