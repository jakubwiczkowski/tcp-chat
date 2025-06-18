#ifndef ACKNOWLEDGE_NAME_H
#define ACKNOWLEDGE_NAME_H

#include "protocol/packet/packet.h"

namespace config::clientbound {

class acknowledge_name final : public packet {
    uint32_t packet_id;
    uint8_t correct;

public:
    explicit acknowledge_name(bytebuf& buffer);
    ~acknowledge_name() override = default;

    void write(bytebuf& buffer) const override;
    [[nodiscard]] uint32_t get_packet_id() const override;

    uint8_t is_correct() const;
};

}

#endif //ACKNOWLEDGE_NAME_H