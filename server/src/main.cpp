#include <iostream>
#include <cstdio>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/socket.h>

#include <iomanip>

#include "chat/chat_server.h"
#include "server/server.h"
#include "src/codec/uint32_codec.h"

// void test() {
//     for (uint32_t test = 891264; test < 891264 + 100; test++) {
//         bytebuf buffer(4);
//         UINT32_CODEC.encode(buffer, test);
//
//         std::cout << "tested value: " << std::dec << test << " (" << std::hex <<
//             test << ")" << std::endl;
//         std::cout << "byte representation: ";
//         for (int i = 0; i < 4; i++) {
//             std::cout << std::hex << std::setw(2) << std::setfill('0') <<
//                 static_cast<unsigned int>(buffer[i]) << " ";
//         }
//         std::cout << std::endl;
//
//         uint32_t decoded = UINT32_CODEC.decode(buffer);
//
//         std::cout << "decoded: " << std::dec << decoded << std::endl;
//         std::cout << (decoded == test) << std::endl << std::endl;
//     }
// }

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

    if (!server.bind_address())
        return 1;

    if (!server.start_listen())
        return 1;

    server.start_listen_thread();
    server.stop();

    return 0;
}