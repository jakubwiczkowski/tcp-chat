#ifndef CLIENT_H
#define CLIENT_H

#include "src/bytebuf/bytebuf.h"
#include <unistd.h>
#include <netinet/in.h>


class client {
    int connfd;
    sockaddr_in address;

public:
    explicit client(int connfd, sockaddr_in address): connfd(connfd), address(address) {}

    ssize_t send(const bytebuf& buffer) const {
        return write(this->connfd, buffer.to_raw().get(), buffer.size());
    }

    [[nodiscard]] int get_connfd() const {
        return this->connfd;
    }

    [[nodiscard]] sockaddr_in get_address() const {
        return this->address;
    }
};



#endif //CLIENT_H
