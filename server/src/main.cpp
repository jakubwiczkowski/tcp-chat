#include <iostream>
#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/socket.h>

#include <iomanip>

#include "server/server.h"
#include "src/codec/uint32_codec.h"

void start_server() {
}

void test() {
    for (uint32_t test = 891264; test < 891264 + 100; test++) {
        bytebuf buffer(4);
        UINT32_CODEC.encode(buffer, test);

        std::cout << "tested value: " << std::dec << test << " (" << std::hex << test << ")" << std::endl;
        std::cout << "byte representation: ";
        for (int i = 0; i < 4; i++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(buffer[i]) << " ";
        }
        std::cout << std::endl;

        uint32_t decoded = UINT32_CODEC.decode(buffer);

        std::cout << "decoded: " << std::dec << decoded << std::endl;
        std::cout << (decoded == test) << std::endl << std::endl;
    }
}

int main() {
    server server(INADDR_ANY, 45678);

    server.set_handling_function([](int connfd, sockaddr_in addr, socklen_t addr_len) {
        unsigned char packet_id_buffer_raw[4];
        ssize_t read_result = read(connfd, packet_id_buffer_raw, sizeof(packet_id_buffer_raw));

        std::cout << "read result: " << read_result << std::endl;

        if (read_result == -1 || read_result == 0) return false;

        bytebuf packet_id_buffer(packet_id_buffer_raw, read_result);

        uint32_t packet_id = UINT32_CODEC.decode(packet_id_buffer);

        std::cout << "PACKET ID: " << packet_id << std::endl;

        return true;
    });

    std::cout << "[#] Tworzenie socketu..." << std::endl;

    if (!server.create_socket()) {
        std::cout << "[!] Nie udało się stworzyć socketu!" << std::endl;
        return 1;
    }

    std::cout << "[#] Socket stworzony pomyślnie!" << std::endl;

    std::cout << "[#] Bindowanie adresu..." << std::endl;

    if (!server.bind_address()) {
        std::cout << "[!] Nie udało się zbindować adresu!" << std::endl;
        return 1;
    }

    std::cout << "[#] Adres zbindowany pomyślnie!" << std::endl;

    std::cout << "[#] Rozpoczynanie nasłuchiwania..." << std::endl;

    if (!server.start_listen()) {
        std::cout << "[!] Nie udało się zacząć nasłuchiwać!" << std::endl;
        return 1;
    }

    std::cout << "[#] Pomyślnie rozpoczęto nasłuchiwanie!" << std::endl;

    server.start_listen_thread();

    server.stop();

    return 0;
}
