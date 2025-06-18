#ifndef SEND_MESSAGE_CLIENT_H
#define SEND_MESSAGE_CLIENT_H

#include "protocol/packet/packet.h"

namespace chat::clientbound {

class send_message final : public packet {
    uint32_t packet_id = 1;
    std::string username;
    std::string message;

public:
    send_message(bytebuf& buffer);
    send_message(std::string username, std::string message);

    ~send_message() override = default;

    void write(bytebuf& buffer) const override;

    [[nodiscard]] uint32_t get_packet_id() const override;

    std::string get_username();
    std::string get_message();
};

}

#endif //SEND_MESSAGE_H
