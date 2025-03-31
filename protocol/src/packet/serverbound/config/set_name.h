#ifndef SET_NAME_H
#define SET_NAME_H

#include "../../packet.h"
#include "src/codec/string_codec.h"
#include "src/codec/uint32_codec.h"

namespace serverbound {

class set_name final : public packet {
    uint32_t packet_id;
    std::string username;

public:
    explicit set_name(bytebuf& buffer): packet(buffer) {
        this->packet_id = UINT32_CODEC.decode(buffer);
        this->username = STRING_CODEC.decode(buffer);
    }

    ~set_name() override;

    void write(bytebuf& buffer) override {
        UINT32_CODEC.encode(buffer, this->packet_id);
        STRING_CODEC.encode(buffer, this->username);
    }

    [[nodiscard]] uint32_t get_packet_id() const override {
        return this->packet_id;
    }

    std::string get_username() {
        return this->username;
    }
};

}

#endif //SET_NAME_H
