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

#include "src/client/client.h"
#include "src/codec/uint32_codec.h"
#include "src/packet/serverbound/chat/send_message.h"
#include "src/packet/serverbound/config/set_name.h"

#define MAX 80
#define PORT 45678
#define SA struct sockaddr

void func(int sockfd) {
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
    std::string username;

    client client(inet_addr("127.0.0.1"), 45678);

    std::cout << "[?] Podaj nazwe uzytkownika: ";
    std::cin >> username;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::unique_ptr<packet> to_send =
                std::make_unique<config::serverbound::set_name>(username);
    client.send_packet(std::move(to_send));

    std::thread chat_thread = std::thread([&] {
        std::string input;

        while (true) {
            std::getline(std::cin, input);

            if (input == "exit") {
                client.stop();
                break;
            }

            std::unique_ptr<packet> to_send =
                std::make_unique<chat::serverbound::send_message>(input);

            client.send_packet(std::move(to_send));
            //TODO: du stuff
        }
    });

    client.receive_loop();
    client.send_loop();

    chat_thread.join();
    // // close the socket
    // close(sockfd);
}