#include <iostream>
#include <thread>

#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#include "src/codec/uint32_codec.h"
#include "src/packet/serverbound/config/set_name.h"

#define MAX 80
#define PORT 45678
#define SA struct sockaddr

void func(int sockfd) {
    // uint32_t packet_id = 15012;
    bytebuf buffer;

    auto set_name_packet = config::serverbound::set_name("test");
    set_name_packet.write(buffer);

    bytebuf final_buffer;
    UINT32_CODEC.encode(final_buffer, buffer.size());
    final_buffer.write(buffer);

    std::cout << "buff: " << final_buffer.size() << std::endl;

    for (;;) {
        auto x = write(sockfd, final_buffer.to_raw().get(), final_buffer.size());
        std::cout << x << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    } else
        printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}