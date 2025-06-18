#ifndef PACKET_H
#define PACKET_H

#include <cstdint>

#include "protocol/bytebuf/bytebuf.h"

class packet {
public:
    virtual ~packet() = default;
    explicit packet() = default;

    virtual void write(bytebuf& buffer) const = 0;

    [[nodiscard]] virtual uint32_t get_packet_id() const = 0;
};

#endif //PACKET_H
