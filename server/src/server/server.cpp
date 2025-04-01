#include "server.h"

#include <strings.h>
#include <unistd.h>

#include <iostream>

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
server<T, E0>::server(uint32_t address, uint16_t port) {
    this->address = address;
    this->port = port;
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
server<T, E0>::~server() {
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
uint32_t server<T, E0>::get_address() const {
    return this->address;
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
uint16_t server<T, E0>::get_port() const {
    return this->port;
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
bool server<T, E0>::create_socket() {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) return false;

    bzero(&this->server_address, sizeof(this->server_address));

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = htonl(this->address);
    this->server_address.sin_port = htons(this->port);

    return true;
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
bool server<T, E0>::bind_address() {
    return bind(this->sockfd,
                reinterpret_cast<sockaddr*>(&this->server_address),
                sizeof(this->server_address)) == 0;
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
bool server<T, E0>::start_listen() const {
    return listen(this->sockfd, 5) == 0;
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
void server<T, E0>::start_listen_thread() {
    this->is_running = true;

    this->listening_thread = std::thread([&] {
        while (this->is_running) {
            sockaddr_in cli;
            socklen_t len;

            int connfd = accept(sockfd, reinterpret_cast<sockaddr*>(&cli), &len);

            if (connfd < 0) continue;

            handle_connection(connfd, cli, len);
        }
    });

    this->listening_thread.join();
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
void server<T, E0>::handle_connection(int connfd, sockaddr_in client_address,
                               socklen_t address_length) {
    T client = this->create_client(connfd, client_address);

    this->client_map.run([&](std::unordered_map<int, T>& value) {
        value.emplace(connfd, client);
    });

    std::thread connection_thread([&] {
        while (this->is_running) {
            bool result = handle_client(client);

            if (!result) {
                this->client_map.run([&](std::unordered_map<int, T>& value) {
                    value.erase(connfd);
                });
                break;
            }
        }
    });

    connection_thread.join();
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
void server<T, E0>::stop() {
    this->is_running = false;
    close(this->sockfd);
}