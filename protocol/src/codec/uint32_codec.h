#ifndef UINT32_CODEC_H
#define UINT32_CODEC_H

#include <cstdint>

#include "codec.h"

class uint32_codec final : public codec<uint32_t> {
public:
    uint32_t decode(bytebuf& buffer) const override {
        if (buffer.remaining() < 4) {
            throw std::out_of_range("Buffer size not enough");
        }

        return (buffer.read() << 24) | (buffer.read() << 16) |
               (buffer.read() << 8) | buffer.read();
    }

    void encode(bytebuf& buffer, uint32_t data) const override {
        buffer.write(data >> 24);
        buffer.write(data >> 16);
        buffer.write(data >> 8);
        buffer.write(data);
    }
};

const static uint32_codec UINT32_CODEC;

#endif //UINT32_CODEC_H