#include "protocol/packet/clientbound/config/acknowledge_name.h"

#include "protocol/codec/uint32_codec.h"
#include "protocol/codec/uint8_codec.h"
#include "protocol/codec/varint_codec.h"

config::clientbound::acknowledge_name::acknowledge_name(bytebuf& buffer) {
    this->packet_id = UINT32_CODEC.decode(buffer);
    this->correct = UINT8_CODEC.decode(buffer);
}

void config::clientbound::acknowledge_name::write(bytebuf& buffer) const {
    UVARINT32_CODEC.encode(buffer, this->packet_id);
    UINT8_CODEC.encode(buffer, this->correct);
}

[[nodiscard]] uint32_t config::clientbound::acknowledge_name::get_packet_id() const {
    return this->packet_id;
}

uint8_t config::clientbound::acknowledge_name::is_correct() const {
    return this->correct;
}