#include "chat_client.h"

state chat_client::get_state() const {
    return this->current_state;
}

void chat_client::set_state(state next_state) {
    this->current_state = next_state;
}

std::string chat_client::get_username(){
    if (this->current_state == CONFIG)
        throw std::runtime_error("get_username cannot be accessed in CONFIG state");

    return this->username;
}