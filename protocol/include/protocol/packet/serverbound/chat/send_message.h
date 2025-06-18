#ifndef SEND_MESSAGE_SERVER_H
#define SEND_MESSAGE_SERVER_H

#include "protocol/packet/packet.h"

namespace chat::serverbound {

class send_message final : public packet {
    uint32_t packet_id = 1;
    std::string message;

public:
    explicit send_message(bytebuf& buffer);
    ~send_message() override = default;

    explicit send_message(std::string message);

    void write(bytebuf& buffer) const override;

    [[nodiscard]] uint32_t get_packet_id() const override;
    std::string get_message();
};

}

#endif //SEND_MESSAGE_SERVER_H
