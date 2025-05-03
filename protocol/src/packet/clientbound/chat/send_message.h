#ifndef SEND_MESSAGE_CLIENT_H
#define SEND_MESSAGE_CLIENT_H

#include "src/codec/string_codec.h"
#include "src/packet/packet.h"

namespace chat::clientbound {

class send_message final : public packet {
    uint32_t packet_id = 1;
    std::string username;
    std::string message;

public:
    send_message(bytebuf& buffer) {
        this->username = STRING_CODEC.decode(buffer);
        this->message = STRING_CODEC.decode(buffer);
    }

    send_message(std::string username, std::string message) {
        this->username = username;
        this->message = message;
    }

    ~send_message() override {
    }

    void write(bytebuf& buffer) override {
        UINT32_CODEC.encode(buffer, this->packet_id);
        STRING_CODEC.encode(buffer, this->username);
        STRING_CODEC.encode(buffer, this->message);
    }

    [[nodiscard]] uint32_t get_packet_id() const override {
        return this->packet_id;
    }

    std::string get_username() {
        return this->username;
    }

    std::string get_message() {
        return this->message;
    }
};

}

#endif //SEND_MESSAGE_H
