#ifndef UINT8_CODEC_H
#define UINT8_CODEC_H

#include "codec.h"

class uint8_codec final : public codec<uint8_t> {
public:
    uint8_t decode(bytebuf& buffer) const override {
        if (buffer.remaining() < 1) {
            throw std::out_of_range("Buffer size not enough");
        }

        return buffer.read();
    }

    void encode(bytebuf& buffer, uint8_t data) const override {
        buffer.write(data);
    }
};

const static uint8_codec UINT8_CODEC;

#endif //UINT8_CODEC_H
