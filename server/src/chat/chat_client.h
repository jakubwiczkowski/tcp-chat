#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include "protocol/packet/state.h"
#include "server/client.h"

class chat_client : public client {
    state current_state = CONFIG;

    std::string username{};

public:
    explicit chat_client(const int connfd,
                         const sockaddr_in addr,
                         const socklen_t addr_len):
        client(connfd, addr, addr_len) {
    }

    [[nodiscard]] state get_state() const;
    void set_state(state next_state);

    std::string get_username();
    void set_username(std::string username);
};


#endif //CHAT_CLIENT_H