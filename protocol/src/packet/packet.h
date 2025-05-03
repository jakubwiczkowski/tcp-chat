#ifndef PACKET_H
#define PACKET_H

#include <cstdint>

#include "src/bytebuf/bytebuf.h"


class packet {
public:
    virtual ~packet() = default;
    explicit packet() = default;

    virtual void write(bytebuf& buffer) = 0;

    virtual uint32_t get_packet_id() const = 0;
};



#endif //PACKET_H
