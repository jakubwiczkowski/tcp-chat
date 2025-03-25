#include <iostream>
#include <thread>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#include "src/codec/uint32_codec.h"
#define MAX 80
#define PORT 45678
#define SA struct sockaddr

void func(int sockfd) {
    uint32_t packet_id = 15012;
    bytebuf buffer;

    UINT32_CODEC.encode(buffer, packet_id);

    std::cout << "buff: " << buffer.size() << std::endl;

    for (;;) {
        auto x = write(sockfd, buffer.to_raw().get(), buffer.size());
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

// int main() {
//     using namespace ftxui;
//
//     std::string server_ip;
//     std::string server_port;
//
//     Component input_server_ip = Input(&server_ip, "IP address");
//     Component input_server_port = Input(&server_port, "Port");
//
//     auto component = Container::Vertical({
//         input_server_ip,
//         input_server_port,
//     });
//
//     auto renderer = Renderer(component, [&] {
//         return vbox({
//                    hbox(
//                        text("Server IP: "),
//                        input_server_ip->Render(),
//                        text(":"),
//                        input_server_port->Render()),
//                    separator(),
//                    text("Hello " + server_ip + " " + server_port),
//                }) |
//                border;
//     });
//
//     auto screen = ScreenInteractive::Fullscreen();
//     screen.Loop(renderer);
//
//     return EXIT_SUCCESS;
// }