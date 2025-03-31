#include "bytebuf.h"

#include <stdexcept>

bytebuf::bytebuf(const byte byte_array[], size_t length): bytebuf(length) {
    for (size_t i = 0; i < length; i++) write(byte_array[i]);
}

bool bytebuf::has_next() const {
    return read_position < write_position;
}

void bytebuf::write(byte byte) {
    ensure_capacity(write_position + 1);
    buffer[write_position++] = byte;
}

void bytebuf::write(const byte bytes[], const size_t length) {
    for (size_t i = 0; i < length; i++) write(bytes[i]);
}

void bytebuf::write(bytebuf& buffer) {
    while (buffer.has_next()) write(buffer.read());
}

byte bytebuf::read() {
    if (read_position >= write_position) {
        throw std::out_of_range("Read beyond buffer");
    }

    return buffer[read_position++];
}

byte bytebuf::peek() const {
    if (read_position >= write_position) {
        throw std::out_of_range("Peek beyond buffer");
    }

    return buffer[read_position];
}

byte bytebuf::peek_at(size_t location) const {
    if (location >= write_position) {
        throw std::out_of_range("Peek at beyond buffer");
    }

    return buffer[location];
}

size_t bytebuf::size() const {
    return write_position;
}

size_t bytebuf::remaining() const {
    return write_position - read_position;
}

std::unique_ptr<byte[]> bytebuf::to_raw() const {
    auto raw = std::make_unique<byte[]>(size());

    for (size_t i = 0; i < size(); i++) raw[i] = peek_at(i);

    return raw;
}

byte bytebuf::operator[](const int idx) const {
    return this->peek_at(idx);
}

void bytebuf::ensure_capacity(size_t new_size) {
    if (new_size <= buffer.size()) return;

    buffer.resize(new_size * 2);
}