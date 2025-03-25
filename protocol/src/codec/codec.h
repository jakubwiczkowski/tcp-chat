#ifndef CODEC_H
#define CODEC_H
#include "src/butebuf/bytebuf.h"


template<typename T>
class codec {
public:
    typedef unsigned char byte;

    virtual ~codec() = default;

    virtual T decode(bytebuf& buffer) const = 0;
    virtual void encode(bytebuf& buffer, T data) const = 0;
};



#endif //CODEC_H
