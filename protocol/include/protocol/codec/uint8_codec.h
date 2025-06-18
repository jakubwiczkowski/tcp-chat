#ifndef UINT8_CODEC_H
#define UINT8_CODEC_H

#include "protocol/codec/codec.h"

class uint8_codec final : public codec<uint8_t> {
public:
    uint8_t decode(bytebuf& buffer) const override;
    void encode(bytebuf& buffer, uint8_t data) const override;
};

const static uint8_codec UINT8_CODEC;

#endif //UINT8_CODEC_H
