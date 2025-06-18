#include "protocol/packet/serverbound/config/set_name.h"

#include "protocol/codec/varint_codec.h"

config::serverbound::set_name::set_name(bytebuf& buffer) {
    // this->packet_id = UINT32_CODEC.decode(buffer);
    this->username = STRING_CODEC.decode(buffer);
}

config::serverbound::set_name::set_name(std::string username) {
    this->username = username;
}

void config::serverbound::set_name::write(bytebuf& buffer) const {
    UVARINT32_CODEC.encode(buffer, this->packet_id);
    STRING_CODEC.encode(buffer, this->username);
}

[[nodiscard]] uint32_t config::serverbound::set_name::get_packet_id() const {
    return this->packet_id;
}

std::string config::serverbound::set_name::get_username() {
    return this->username;
}
