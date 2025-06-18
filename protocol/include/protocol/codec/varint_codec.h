#ifndef VARINT_CODEC_H
#define VARINT_CODEC_H

#include "codec.h"

template<class T>
class varint_codec final : public codec<T> {
    static constexpr uint8_t SEGMENT_BITS = 0x7F;
    static constexpr uint8_t CONTINUE_BIT = 0x80;
public:
    T decode(bytebuf& buffer) const override;
    void encode(bytebuf& buffer, T data) const override;
};

template class varint_codec<int32_t>;
template class varint_codec<uint32_t>;

const static varint_codec<int32_t> SVARINT32_CODEC;
const static varint_codec<uint32_t> UVARINT32_CODEC;

#endif //VARINT_CODEC_H
