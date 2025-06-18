#include "chat_server.h"

#include "chat_client.h"
#include "protocol/codec/uint32_codec.h"
#include "protocol/codec/varint_codec.h"
#include "protocol/packet/clientbound/chat/send_message.h"
#include "protocol/packet/serverbound/chat/send_message.h"
#include "protocol/packet/serverbound/config/set_name.h"

bool chat_server::handle_client(chat_client& client) {
    byte packet_length_buffer_raw[4];
    ssize_t read_result = read(client.get_connfd(), packet_length_buffer_raw,
                               sizeof(packet_length_buffer_raw));

    if (read_result == -1 || read_result == 0) return false;

    bytebuf packet_length_buffer(packet_length_buffer_raw, read_result);
    uint32_t packet_length = UINT32_CODEC.decode(packet_length_buffer);

    std::unique_ptr<byte[]> data = std::unique_ptr<byte[]>(
        new byte[packet_length]);

    read_result = read(client.get_connfd(), data.get(), packet_length);

    bytebuf data_buffer(data.get(), read_result);
    uint32_t packet_id = UVARINT32_CODEC.decode(data_buffer);

    switch (packet_id) {
        case 0: {
            if (client.get_state() == CHAT) break;

            config::serverbound::set_name set_name_packet(data_buffer);
            client.set_state(CHAT);
            client.set_username(set_name_packet.get_username());

            this->get_logger().log(logger::level::INFO, "Client " +
                                       std::to_string(client.get_connfd()) +
                                       " (" + client.get_address_readable() +
                                       ") uses username: " +
                                       set_name_packet.get_username());

            break;
        }
        case 1: {
            if (client.get_state() != CHAT) break;

            chat::serverbound::send_message send_message_packet(data_buffer);

            this->get_logger().log(logger::level::INFO,
                                   client.get_username() + " (" + client.
                                   get_address_readable() + "): " +
                                   send_message_packet.get_message());

            this->client_map.run(
                [&client, &send_message_packet](
                std::unordered_map<int, chat_client>& value) {
                    for (auto [_, target_client] : value) {
                        std::unique_ptr<chat::clientbound::send_message> to_send
                            =
                            std::make_unique<chat::clientbound::send_message>(
                                client.get_username(),
                                send_message_packet.get_message());

                        target_client.send_packet(std::move(to_send));
                    }
                });

            break;
        }
        default: {
            break;
        }
    }

    return true;
}

chat_client chat_server::create_client(const int connfd,
                                       const sockaddr_in addr,
                                       const socklen_t addr_len) {
    this->get_logger().log(logger::level::INFO,
                           "Created client with connfd " + std::to_string(
                               connfd));
    return chat_client(connfd, addr, addr_len);
}