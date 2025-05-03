#include "client.h"

#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "src/codec/uint32_codec.h"

client::client(uint32_t address, uint16_t port) {
    sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(45678);

    if (connect(sockfd, reinterpret_cast<sockaddr*>(&servaddr), sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }

    printf("connected to the server..\n");
}

void client::send_packet(packet& packet) {
    this->packet_queue.run([&](auto value) {
        value.push(packet);
    });
    // bytebuf buffer;
    // packet.write(buffer);
    //
    // bytebuf final_buffer;
    // UINT32_CODEC.encode(final_buffer, buffer.size());
    // final_buffer.write(buffer);
    //
    // auto x = write(sockfd, final_buffer.to_raw().get(), final_buffer.size());
}