#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <cstdint>
#include <functional>
#include <thread>

#define SA struct sockaddr

class server {
public:
    typedef std::function<bool(int, sockaddr_in, socklen_t)> handle_fn;

private:
    uint32_t address;
    uint16_t port;

    sockaddr_in server_address;

    int sockfd;

    std::thread listening_thread;

    handle_fn connection_handle_fn;

public:
    server(uint32_t address, uint16_t port);
    ~server();

    void set_handling_function(handle_fn handle_fn);

    [[nodiscard]] uint32_t get_address() const;
    [[nodiscard]] uint16_t get_port() const;

    bool create_socket();
    bool bind_address();
    bool start_listen() const;
    void start_listen_thread();

    void handle_connection(int connfd, sockaddr_in client_address, socklen_t address_length) const;

    void stop() const;
};



#endif //SERVER_H
