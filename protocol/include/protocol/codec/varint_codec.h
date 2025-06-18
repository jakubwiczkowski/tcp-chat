#ifndef VARINT_CODEC_H
#define VARINT_CODEC_H

#include "codec.h"

template<class T>
class varint_codec final : public codec<T> {
    static constexpr uint8_t SEGMENT_BITS = 0x7F;
    static constexpr uint8_t CONTINUE_BIT = 0x80;
public:
    T decode(bytebuf& buffer) const override {
        T value = 0;
        uint8_t position = 0;
        byte currentByte;

        while (true) {
            currentByte = buffer.read();
            value |= (currentByte & SEGMENT_BITS) << position;

            if ((currentByte & CONTINUE_BIT) == 0) break;

            position += 7;
            if (position >= 32) throw std::out_of_range("VarInt is too big");
        }

        return value;
    }

    void encode(bytebuf& buffer, T data) const override {
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
};

template class varint_codec<int32_t>;
template class varint_codec<uint32_t>;

const static varint_codec<int32_t> SVARINT32_CODEC;
const static varint_codec<uint32_t> UVARINT32_CODEC;

#endif //VARINT_CODEC_H
