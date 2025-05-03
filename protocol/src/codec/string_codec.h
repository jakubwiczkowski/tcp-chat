#ifndef STRING_CODEC_H
#define STRING_CODEC_H
#include <memory>
#include <string>

#include "codec.h"
#include "uint32_codec.h"

class string_codec : public codec<std::string> {
public:
    std::string decode(bytebuf& buffer) const override {
        // strings are prepended with uint32_t length

        if (buffer.remaining() < 4) {
            throw std::out_of_range("Buffer size not enough (to read length)");
        }

        uint32_t length = UINT32_CODEC.decode(buffer);

        if (buffer.remaining() < length) {
            throw std::out_of_range("Buffer size not enough (no space in buffer)");
        }

        std::string result;

        for (size_t i = 0; i < length; i++)
            result.push_back(static_cast<char>(buffer.read()));

        return result;
    }

    void encode(bytebuf& buffer, std::string data) const override {
        bytebuf length_buffer;
        UINT32_CODEC.encode(length_buffer, data.length());

        buffer.write(length_buffer);

        for (const char c : data) buffer.write(c);
    }
};

const static string_codec STRING_CODEC;

#endif //STRING_CODEC_H
