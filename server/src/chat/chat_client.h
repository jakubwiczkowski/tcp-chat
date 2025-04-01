#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include "src/server/client.h"

class chat_client : public client {
public:
    explicit chat_client(int connfd, sockaddr_in addr): client(connfd, addr) {
    }
};


#endif //CHAT_CLIENT_H