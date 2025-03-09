#include <iostream>
#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/socket.h>

#include "server/server.h"
#define MAX 80

int main() {
    server server(INADDR_ANY, 45678);

    server.set_handling_function([](int connfd, sockaddr_in addr, socklen_t addr_len) {
        char packet_id[1];
        ssize_t read_result = read(connfd, packet_id, sizeof(packet_id));

        if (read_result == -1) return false;

        std::cout << "PACKET ID: " << +packet_id[0] << std::endl;

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
    // int sockfd, connfd;
    // socklen_t len;
    // sockaddr_in servaddr, cli;
    //
    // // Now server is ready to listen and verification
    //
    // // Accept the data packet from client and verification
    // connfd = accept(sockfd, (SA*)&cli, &len);
    // if (connfd < 0) {
    //     printf("server accept failed...\n");
    //     exit(0);
    // }
    // else
    //     printf("server accept the client...\n");
    //
    // // Function for chatting between client and server
    // func(connfd);
    //
    // // After chatting close the socket
    // close(sockfd);
}
