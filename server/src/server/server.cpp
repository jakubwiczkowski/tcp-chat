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

template <typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
logger& server<T, E0>::get_logger() {
    return this->server_logger;
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
    this->server_logger.log(logger::level::INFO, "Creating socket...");
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        this->server_logger.log(logger::level::ERROR, "Socket creation failed!");
        return false;
    }

    this->server_logger.log(logger::level::INFO, "Socket created.");

    bzero(&this->server_address, sizeof(this->server_address));

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = htonl(this->address);
    this->server_address.sin_port = htons(this->port);

    return true;
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
bool server<T, E0>::bind_address() {
    this->server_logger.log(logger::level::INFO, "Binding address...");
    return bind(this->sockfd,
                reinterpret_cast<sockaddr*>(&this->server_address),
                sizeof(this->server_address)) == 0;
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
bool server<T, E0>::start_listen() {
    this->server_logger.log(logger::level::INFO, "Starting to listen...");
    return listen(this->sockfd, 5) == 0;
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
void server<T, E0>::start_listen_thread() {
    this->server_logger.log(logger::level::INFO, "Starting listening thread...");
    this->is_running = true;

    this->listening_thread = std::thread([&] {
        while (this->is_running) {
            sockaddr_in cli = {};
            socklen_t len = sizeof(cli);

            int connfd = accept(sockfd, reinterpret_cast<sockaddr*>(&cli), &len);

            if (connfd < 0) continue;

            this->server_logger.log(logger::level::INFO, "Incoming connection on connfd " + std::to_string(connfd));
            handle_connection(connfd, cli, len);
        }
        this->server_logger.log(logger::level::INFO, "Listening thread stopped.");
    });

    this->listening_thread.join();
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
void server<T, E0>::handle_connection(int connfd, sockaddr_in client_address,
                               socklen_t address_length) {
    this->get_logger().log(logger::level::INFO, "Establishing new connection...");

    T client = this->create_client(connfd, client_address, address_length);

    this->client_map.run([&](std::unordered_map<int, T>& value) {
        value.emplace(connfd, client);
    });

    this->get_logger().log(logger::level::INFO,
        "Established connection with connfd " + std::to_string(connfd) +
        " (" + client.get_address_readable() + ")");

    std::thread connection_thread([connfd, this] {
        T& connection_client = this->client_map.run_and_return([connfd](std::unordered_map<int, T>& value) -> T& {
            return value.at(connfd);
        });

        while (this->is_running) {
            bool result = handle_client(connection_client);

            if (!result) {
                this->get_logger().log(logger::level::INFO,
                    "Connection with connfd " + std::to_string(connfd) +
                    " (" + connection_client.get_address_readable() + ") closed.");
                this->client_map.run([&](std::unordered_map<int, T>& value) {
                    value.erase(connfd);
                });
                break;
            }
        }
    });

    connection_thread.detach();
}

template<typename T, std::enable_if_t<std::is_base_of_v<client, T>>* E0>
void server<T, E0>::stop() {
    this->is_running = false;
    close(this->sockfd);
}