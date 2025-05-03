#include "client.h"

#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <iostream>
#include <thread>

#include "src/codec/uint32_codec.h"
#include "src/packet/clientbound/chat/send_message.h"

client::client(uint32_t address, uint16_t port) {
    sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        this->client_logger.log(logger::level::ERROR, "Socket creation failed");
        exit(0);
    }

    this->client_logger.log(logger::level::INFO, "Socket successfully created");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = address;
    servaddr.sin_port = htons(port);

    if (connect(sockfd, reinterpret_cast<sockaddr*>(&servaddr),
                sizeof(servaddr))
        != 0) {
        this->client_logger.log(logger::level::ERROR,
                                "Connection with the server failed");
        exit(0);
    }

    this->client_logger.log(logger::level::INFO, "Connected to the server!");
    this->is_running = true;
}

void client::send_packet(std::unique_ptr<packet> to_send) {
    this->packet_queue.run(
        [to_send = std::move(to_send)]
    (std::queue<std::unique_ptr<packet>>& queue) mutable {
            queue.push(std::move(to_send));
        }
        );
}

void client::receive_loop() {
    std::thread receive_loop = std::thread([&] {
        while (this->is_running) {
            byte packet_length_buffer_raw[4];
            ssize_t read_result = read(this->sockfd, packet_length_buffer_raw,
                                       sizeof(packet_length_buffer_raw));

            if (read_result == -1 || read_result == 0) {
                stop();
                return;
            }

            bytebuf packet_length_buffer(packet_length_buffer_raw, read_result);
            uint32_t packet_length = UINT32_CODEC.decode(packet_length_buffer);

            std::unique_ptr<byte[]> data = std::unique_ptr<byte[]>(
                new byte[packet_length]);

            read_result = read(this->sockfd, data.get(), packet_length);

            bytebuf data_buffer(data.get(), read_result);
            uint32_t packet_id = UINT32_CODEC.decode(data_buffer);

            switch (packet_id) {
                case 1: {
                    chat::clientbound::send_message received(data_buffer);

                    std::cout << received.get_username() << ": " << received.
                        get_message() << "\n";

                    break;
                }
                default: {
                    break;
                }
            }
        }
    });
    receive_loop.detach();
}

void client::send_loop() {
    std::thread send_loop = std::thread([&] {
        while (this->is_running) {
            if (this->packet_queue.get().empty()) continue;

            std::unique_ptr<packet> to_send;

            this->packet_queue.run(
                [&](std::queue<std::unique_ptr<packet>>& queue) mutable {
                    to_send = std::move(queue.front());
                    queue.pop();
                });

            bytebuf buffer;
            to_send->write(buffer);

            bytebuf final_buffer;
            UINT32_CODEC.encode(final_buffer, buffer.size());
            final_buffer.write(buffer);

            auto x = write(sockfd, final_buffer.to_raw().get(),
                           final_buffer.size());
        }
    });
    send_loop.join();
}

bool client::is_client_running() const {
    return this->is_running;
}

void client::stop() {
    this->is_running = false;
}