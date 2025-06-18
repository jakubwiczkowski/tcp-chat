#ifndef UINT32_CODEC_H
#define UINT32_CODEC_H

#include <cstdint>

#include "protocol/codec/codec.h"

class uint32_codec final : public codec<uint32_t> {
public:
    uint32_t decode(bytebuf& buffer) const override;
    void encode(bytebuf& buffer, uint32_t data) const override;
};

const static uint32_codec UINT32_CODEC;

#endif //UINT32_CODEC_H