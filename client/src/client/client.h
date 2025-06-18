#ifndef CLIENT_H
#define CLIENT_H
#include <cstdint>
#include <queue>

#include "common/logger/logger.h"
#include "common/sync/sync.h"
#include "protocol/packet/packet.h"

class client {
    int sockfd;
    bool is_running = false;

    logger client_logger{};
    synced<std::queue<std::unique_ptr<packet>>> packet_queue = {};
public:
    explicit client(uint32_t address, uint16_t port);

    void send_packet(const packet& to_send) const;

    void receive_loop();
    void send_loop();

    bool is_client_running() const;

    void stop();
};



#endif //CLIENT_H
