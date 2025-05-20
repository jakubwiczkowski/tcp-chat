#include <iostream>
#include <cstdio>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/socket.h>
#include <csignal>
#include <cstdlib>

#include <iomanip>

#include "chat/chat_server.h"
#include "server/server.h"

std::function<void(int)> shutdown_handler;
void signal_handler(int signal) { shutdown_handler(signal); }

int main(int argc, char* argv[]) {
    uint32_t default_address = INADDR_ANY;
    uint16_t default_port = 45678;

    if (argc >= 2) {
        default_address = inet_pton(AF_INET, argv[1], &default_address);

        if (argc == 3)
            default_port = atoi(argv[2]);
    }

    chat_server server(default_address, default_port);

    if (!server.create_socket())
        return 1;

    if (!server.bind_address()) {
        server.get_logger().log(logger::level::ERROR,
            "Failed to bind address");
        return 1;
    }

    if (!server.start_listen()) {
        server.get_logger().log(logger::level::ERROR,
            "Failed to start listening thread");
        return 1;
    }

    std::signal(SIGINT, signal_handler);
    std::signal(SIGSEGV, signal_handler);
    std::signal(SIGTERM, signal_handler);
    std::signal(SIGKILL, signal_handler);
    shutdown_handler = [&](int signal) {
        server.get_logger().log(logger::level::INFO, "Server shutting down... (sig: " + std::to_string(signal) + ")");
        server.stop();
        exit(0);
    };

    server.start_listen_thread();
    server.stop();

    return 0;
}