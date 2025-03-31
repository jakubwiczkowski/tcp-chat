#ifndef ACKNOWLEDGE_NAME_H
#define ACKNOWLEDGE_NAME_H
#include "src/codec/uint32_codec.h"
#include "src/codec/uint8_codec.h"
#include "src/packet/packet.h"

namespace clientbound {

class acknowledge_name final : public packet {
    uint32_t packet_id;
    uint8_t correct;

public:
    explicit acknowledge_name(bytebuf& buffer): packet(buffer) {
        this->packet_id = UINT32_CODEC.decode(buffer);
        this->correct = UINT8_CODEC.decode(buffer);
    }

    ~acknowledge_name() override;

    void write(bytebuf& buffer) override {
        UINT32_CODEC.encode(buffer, this->packet_id);
        UINT8_CODEC.encode(buffer, this->correct);
    }

    [[nodiscard]] uint32_t get_packet_id() const override {
        return this->packet_id;
    }

    uint8_t is_correct() const {
        return this->correct;
    }
};

}

#endif //ACKNOWLEDGE_NAME_H