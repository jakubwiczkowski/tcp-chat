#include "chat_server.h"

#include <iostream>

#include "chat_client.h"
#include "src/codec/uint32_codec.h"

bool chat_server::handle_client(chat_client& client) {
    unsigned char packet_id_buffer_raw[4];
    ssize_t read_result = read(client.get_connfd(), packet_id_buffer_raw,
                               sizeof(packet_id_buffer_raw));

    std::cout << "read result: " << read_result << std::endl;

    if (read_result == -1 || read_result == 0) return false;

    bytebuf packet_id_buffer(packet_id_buffer_raw, read_result);

    uint32_t packet_id = UINT32_CODEC.decode(packet_id_buffer);

    std::cout << "PACKET ID: " << packet_id << std::endl;

    return true;
}

chat_client chat_server::create_client(int connfd, sockaddr_in addr) {
    std::cout << "create client " << connfd << std::endl;
    return chat_client(connfd, addr);
}