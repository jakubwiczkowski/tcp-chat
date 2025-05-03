#ifndef CLIENT_H
#define CLIENT_H

#include "src/bytebuf/bytebuf.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "src/codec/uint32_codec.h"
#include "src/packet/packet.h"


class client {
    int connfd;
    sockaddr_in address;
    socklen_t address_length;
    std::string address_human_readable;

public:
    explicit client(int connfd, sockaddr_in address, socklen_t address_length):
        connfd(connfd),
        address(address),
        address_length(address_length),
        address_human_readable(sockaddr_to_string(address)) {
    }

    ssize_t send(const bytebuf& buffer) const {
        return write(this->connfd, buffer.to_raw().get(), buffer.size());
    }

    void send_packet(std::unique_ptr<packet> to_send) {
        bytebuf buffer;
        to_send->write(buffer);

        bytebuf final_buffer;
        UINT32_CODEC.encode(final_buffer, buffer.size());
        final_buffer.write(buffer);

        send(final_buffer);
    }

    [[nodiscard]] int get_connfd() const {
        return this->connfd;
    }

    [[nodiscard]] sockaddr_in get_address() const {
        return this->address;
    }

    [[nodiscard]] std::string get_address_readable() {
        return this->address_human_readable;
    }

    static std::string sockaddr_to_string(const sockaddr_in addr) {
        char ipStr[INET_ADDRSTRLEN] = {};
        inet_ntop(AF_INET, &addr.sin_addr, ipStr, INET_ADDRSTRLEN);
        return std::string(ipStr) + ":" + std::to_string(ntohs(addr.sin_port));
    }
};


#endif //CLIENT_H