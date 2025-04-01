#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <cstdint>
#include <functional>
#include <thread>

#include "client.h"
#include "src/chat/chat_client.h"
#include "src/logger/logger.h"
#include "src/sync/sync.h"

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* = nullptr>
class server {
    uint32_t address;
    uint16_t port;

    sockaddr_in server_address;

    int sockfd;

    std::thread listening_thread;

    std::atomic_bool is_running = false;

    logger server_logger{};
    synced<std::unordered_map<int, T>> client_map;

public:
    server(uint32_t address, uint16_t port);
    virtual ~server();

    logger& get_logger();

    virtual bool handle_client(T& client) = 0;
    virtual T create_client(int connfd, sockaddr_in addr, socklen_t addr_len) = 0;

    [[nodiscard]] uint32_t get_address() const;
    [[nodiscard]] uint16_t get_port() const;

    bool create_socket();
    bool bind_address();
    bool start_listen();
    void start_listen_thread();

    void handle_connection(int connfd, sockaddr_in client_address, socklen_t address_length);

    void stop();
};

template class server<chat_client>;

#endif //SERVER_H
