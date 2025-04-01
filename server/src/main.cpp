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
        // std::cout << "[#] Adres: " << argv[1] << " (" << default_address << ")" << std::endl;

        if (argc == 3) {
            default_port = atoi(argv[2]);
            // std::cout << "[#] Port: " << default_port << std::endl;
        }
    }

    chat_server server(default_address, default_port);

    // std::cout << "[#] Tworzenie socketu..." << std::endl;

    if (!server.create_socket()) {
        // std::cout << "[!] Nie udało się stworzyć socketu!" << std::endl;
        return 1;
    }

    // std::cout << "[#] Socket stworzony pomyślnie!" << std::endl;

    // std::cout << "[#] Bindowanie adresu..." << std::endl;

    if (!server.bind_address()) {
        // std::cout << "[!] Nie udało się zbindować adresu!" << std::endl;
        return 1;
    }

    // std::cout << "[#] Adres zbindowany pomyślnie!" << std::endl;

    // std::cout << "[#] Rozpoczynanie nasłuchiwania..." << std::endl;

    if (!server.start_listen()) {
        // std::cout << "[!] Nie udało się zacząć nasłuchiwać!" << std::endl;
        return 1;
    }

    // std::cout << "[#] Pomyślnie rozpoczęto nasłuchiwanie!" << std::endl;

    server.start_listen_thread();

    server.stop();

    return 0;
}