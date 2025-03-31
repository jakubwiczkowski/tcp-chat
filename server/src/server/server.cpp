#include "server.h"

#include <strings.h>
#include <unistd.h>

server::server(uint32_t address, uint16_t port) {
    this->address = address;
    this->port = port;
}

server::~server() {
}

void server::set_handling_function(handle_fn handle_fn) {
    this->connection_handle_fn = std::move(handle_fn);
}

void server::set_client_create_function(client_fn client_fn) {
    this->client_create_fn = std::move(client_fn);
}

uint32_t server::get_address() const {
    return this->address;
}

uint16_t server::get_port() const {
    return this->port;
}

bool server::create_socket() {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) return false;

    bzero(&this->server_address, sizeof(this->server_address));

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = htonl(this->address);
    this->server_address.sin_port = htons(this->port);

    return true;
}

bool server::bind_address() {
    return bind(this->sockfd,
                reinterpret_cast<sockaddr*>(&this->server_address),
                sizeof(this->server_address)) == 0;
}

bool server::start_listen() const {
    return listen(this->sockfd, 5) == 0;
}

void server::start_listen_thread() {
    this->is_running = true;

    this->listening_thread = std::thread([&] {
        while (this->is_running) {
            sockaddr_in cli;
            socklen_t len;

            int connfd = accept(sockfd, reinterpret_cast<sockaddr*>(&cli), &len);

            // std::cout << connfd << std::endl;

            if (connfd < 0) continue;

            handle_connection(connfd, cli, len);
        }
    });

    this->listening_thread.join();
}

void server::handle_connection(int connfd, sockaddr_in client_address,
                               socklen_t address_length) {
    this->client_map.run([&](std::unordered_map<int, client>& value) {
        value.emplace(connfd, this->client_create_fn(connfd, client_address));
    });

    std::thread connection_thread([&] {
        while (this->is_running) {
            bool result = connection_handle_fn(connfd, client_address, address_length);

            if (!result) {
                this->client_map.run([&](std::unordered_map<int, client>& value) {
                    value.erase(connfd);
                });
                break;
            }
        }
    });

    connection_thread.join();
}

void server::stop() {
    this->is_running = false;
    close(this->sockfd);
}