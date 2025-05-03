#ifndef CLIENT_H
#define CLIENT_H
#include <cstdint>
#include <queue>

#include "src/packet/packet.h"
#include "src/sync/sync.h"

class client {
    int sockfd;

    synced<std::queue<packet>> packet_queue = {};
public:
    explicit client(uint32_t address, uint16_t port);

    void send_packet(packet& packet);

    void receive_loop();
    void send_loop();
};



#endif //CLIENT_H
