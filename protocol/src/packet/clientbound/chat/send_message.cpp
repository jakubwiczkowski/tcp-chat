#include "protocol/packet/clientbound/chat/send_message.h"

#include "protocol/codec/string_codec.h"
#include "protocol/codec/uint32_codec.h"

chat::clientbound::send_message::send_message(bytebuf& buffer) {
    this->username = STRING_CODEC.decode(buffer);
    this->message = STRING_CODEC.decode(buffer);
}

chat::clientbound::send_message::send_message(std::string username, std::string message) {
    this->username = username;
    this->message = message;
}

void chat::clientbound::send_message::write(bytebuf& buffer) const {
    UINT32_CODEC.encode(buffer, this->packet_id);
    STRING_CODEC.encode(buffer, this->username);
    STRING_CODEC.encode(buffer, this->message);
}

[[nodiscard]] uint32_t chat::clientbound::send_message::get_packet_id() const {
    return this->packet_id;
}

std::string chat::clientbound::send_message::get_username() {
    return this->username;
}

std::string chat::clientbound::send_message::get_message() {
    return this->message;
}
