#include <iostream>
#include <thread>

#include <arpa/inet.h>

#include "client/client.h"
#include "protocol/packet/serverbound/chat/send_message.h"
#include "protocol/packet/serverbound/config/set_name.h"

int main() {
    std::string username;

    client client(inet_addr("127.0.0.1"), 45678);

    std::cout << "[?] Podaj nazwe uzytkownika: ";
    std::cin >> username;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    client.send_packet(config::serverbound::set_name(username));

    std::thread chat_thread = std::thread([&] {
        std::string input;

        while (client.is_client_running()) {
            std::getline(std::cin, input);

            if (input == "exit") {
                client.stop();
                break;
            }

            client.send_packet(chat::serverbound::send_message(input));
        }
    });

    client.receive_loop();

    chat_thread.join();
}