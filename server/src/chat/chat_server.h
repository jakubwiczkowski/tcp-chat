#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H
#include "chat_client.h"
#include "src/server/server.h"


class chat_server final : public server<chat_client> {
public:
    chat_server(uint32_t address, uint16_t port)
        : server(address, port) {
    }

    bool handle_client(chat_client& client) override;
    chat_client create_client(int connfd, sockaddr_in addr) override;
};


#endif //CHAT_SERVER_H