#ifndef SET_NAME_H
#define SET_NAME_H

#include "../../packet.h"
#include "src/codec/string_codec.h"
#include "src/codec/uint32_codec.h"

namespace config::serverbound {

class set_name final : public packet {
    uint32_t packet_id = 0;
    std::string username;

public:
    set_name(bytebuf& buffer) {
        // this->packet_id = UINT32_CODEC.decode(buffer);
        this->username = STRING_CODEC.decode(buffer);
    }

    set_name(std::string username) {
        this->username = username;
    }

    ~set_name() override {
    }

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
