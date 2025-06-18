#ifndef STRING_CODEC_H
#define STRING_CODEC_H

#include <string>

#include "protocol/codec/codec.h"

class string_codec : public codec<std::string> {
public:
    std::string decode(bytebuf& buffer) const override;
    void encode(bytebuf& buffer, std::string data) const override;
};

const static string_codec STRING_CODEC;

#endif //STRING_CODEC_H