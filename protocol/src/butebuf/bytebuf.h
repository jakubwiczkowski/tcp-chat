#ifndef BYTEBUF_H
#define BYTEBUF_H

#include <memory>
#include <vector>

typedef unsigned char byte;

class bytebuf {
    std::vector<byte> buffer;

    size_t read_position = 0;
    size_t write_position = 0;

public:
    explicit bytebuf(const size_t capacity = 8) : buffer(capacity) {}
    explicit bytebuf(const byte byte_array[], size_t length);

    [[nodiscard]] bool has_next() const;

    void write(byte byte);
    void write(const byte bytes[], size_t length);
    void write(bytebuf& buffer);

    [[nodiscard]] byte read();
    [[nodiscard]] byte peek() const;
    [[nodiscard]] byte peek_at(size_t location) const;

    [[nodiscard]] size_t size() const;
    [[nodiscard]] size_t remaining() const;

    [[nodiscard]] std::unique_ptr<byte[]> to_raw() const;

    byte operator[](int idx) const;

private:
    void ensure_capacity(size_t new_size);
};


#endif //BYTEBUF_H
