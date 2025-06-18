#include "protocol/codec/varint_codec.h"

template<class T>
T varint_codec<T>::decode(bytebuf& buffer) const {
    T value = 0;
    uint8_t position = 0;
    byte current_byte;

    while (true) {
        current_byte = buffer.read();
        value |= (current_byte & SEGMENT_BITS) << position;

        if ((current_byte & CONTINUE_BIT) == 0) break;

        position += 7;
        if (position >= 32) throw std::out_of_range("VarInt is too big");
    }

    return value;
}

template<class T>
void varint_codec<T>::encode(bytebuf& buffer, T data) const {
    auto unsigned_data = static_cast<uint32_t>(data);
    while (true) {
        if ((unsigned_data & ~SEGMENT_BITS) == 0) {
            buffer.write(static_cast<byte>(unsigned_data));
            return;
        }

        buffer.write(static_cast<byte>(unsigned_data) & SEGMENT_BITS | CONTINUE_BIT);
        unsigned_data >>= 7;
    }
}
