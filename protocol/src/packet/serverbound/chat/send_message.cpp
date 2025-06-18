#include "protocol/packet/serverbound/chat/send_message.h"

#include "protocol/codec/string_codec.h"
#include "protocol/codec/uint32_codec.h"

chat::serverbound::send_message::send_message(bytebuf& buffer) {
    // this->packet_id = UINT32_CODEC.decode(buffer);
    this->message = STRING_CODEC.decode(buffer);
}

chat::serverbound::send_message::send_message(std::string message) {
    this->message = message;
}

void chat::serverbound::send_message::write(bytebuf& buffer) const {
    UINT32_CODEC.encode(buffer, this->packet_id);
    STRING_CODEC.encode(buffer, this->message);
}

[[nodiscard]] uint32_t chat::serverbound::send_message::get_packet_id() const {
    return this->packet_id;
}

std::string chat::serverbound::send_message::get_message() {
    return this->message;
}
