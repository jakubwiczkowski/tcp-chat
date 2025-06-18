#ifndef SET_NAME_H
#define SET_NAME_H

#include "protocol/packet/packet.h"
#include "protocol/codec/string_codec.h"
#include "protocol/codec/uint32_codec.h"

namespace config::serverbound {

class set_name final : public packet {
    uint32_t packet_id = 0;
    std::string username;

public:
    explicit set_name(bytebuf& buffer);
    ~set_name() override = default;

    explicit set_name(std::string username);

    void write(bytebuf& buffer) const override;
    [[nodiscard]] uint32_t get_packet_id() const override;

    std::string get_username();
};

}

#endif //SET_NAME_H
