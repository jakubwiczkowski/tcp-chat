#include <catch2/catch_test_macros.hpp>

#include "catch2/generators/catch_generators.hpp"
#include "catch2/generators/catch_generators_all.hpp"

#include "protocol/codec/string_codec.h"
#include "protocol/bytebuf/bytebuf.h"
#include "protocol/codec/uint32_codec.h"
#include "protocol/codec/uint8_codec.h"
#include "protocol/codec/varint_codec.h"

TEST_CASE("Signed VarInt is encoded correctly") {
    const varint_codec<int32_t> codec;
    bytebuf buffer;

    SECTION("1 byte VarInt") {
        int32_t test_int = GENERATE(0, 127);
        codec.encode(buffer, test_int);

        REQUIRE(codec.decode(buffer) == test_int);
        REQUIRE(buffer.size() == 1);
    }

    SECTION("2 byte VarInt") {
        int32_t test_int = GENERATE(128, 16383);
        codec.encode(buffer, test_int);

        REQUIRE(codec.decode(buffer) == test_int);
        REQUIRE(buffer.size() == 2);
    }

    SECTION("3 byte VarInt") {
        int32_t test_int = GENERATE(16384, 2097151);
        codec.encode(buffer, test_int);

        REQUIRE(codec.decode(buffer) == test_int);
        REQUIRE(buffer.size() == 3);
    }

    SECTION("4 byte VarInt") {
        int32_t test_int = GENERATE(2097152, 268435455);
        codec.encode(buffer, test_int);

        REQUIRE(codec.decode(buffer) == test_int);
        REQUIRE(buffer.size() == 4);
    }

    SECTION("5 byte VarInt positive") {
        int32_t test_int = GENERATE(268435456, 4294967295);
        codec.encode(buffer, test_int);

        REQUIRE(codec.decode(buffer) == test_int);
        REQUIRE(buffer.size() == 5);
    }

    SECTION("5 byte VarInt negative") {
        int32_t test_int = GENERATE(-2147483648, -1);
        codec.encode(buffer, test_int);

        REQUIRE(codec.decode(buffer) == test_int);
        REQUIRE(buffer.size() == 5);
    }
}

TEST_CASE("Unsigned VarInt is encoded correctly") {
    const varint_codec<uint32_t> codec;
    bytebuf buffer;

    SECTION("1 byte VarInt") {
        uint32_t test_int = GENERATE(0, 127);
        codec.encode(buffer, test_int);

        REQUIRE(codec.decode(buffer) == test_int);
        REQUIRE(buffer.size() == 1);
    }

    SECTION("2 byte VarInt") {
        uint32_t test_int = GENERATE(128, 16383);
        codec.encode(buffer, test_int);

        REQUIRE(codec.decode(buffer) == test_int);
        REQUIRE(buffer.size() == 2);
    }

    SECTION("3 byte VarInt") {
        uint32_t test_int = GENERATE(16384, 2097151);
        codec.encode(buffer, test_int);

        REQUIRE(codec.decode(buffer) == test_int);
        REQUIRE(buffer.size() == 3);
    }

    SECTION("4 byte VarInt") {
        uint32_t test_int = GENERATE(2097152, 268435455);
        codec.encode(buffer, test_int);

        REQUIRE(codec.decode(buffer) == test_int);
        REQUIRE(buffer.size() == 4);
    }

    SECTION("5 byte VarInt") {
        uint32_t test_int = GENERATE(268435456, 4294967295);
        codec.encode(buffer, test_int);

        REQUIRE(codec.decode(buffer) == test_int);
        REQUIRE(buffer.size() == 5);
    }
}

TEST_CASE("UINT8 is encoded correctly") {
    const uint8_codec codec;
    bytebuf buffer;

    SECTION("Valid UINT8") {
        uint8_t test_int = GENERATE(range(0, 255));
        codec.encode(buffer, test_int);

        REQUIRE(buffer.size() == 1);
        REQUIRE(codec.decode(buffer) == test_int);
    }
}

TEST_CASE("UINT32 is encoded correctly") {
    const uint32_codec codec;
    bytebuf buffer;

    SECTION("Valid UINT32") {
        uint32_t test_int = GENERATE(range(0, 1023));
        codec.encode(buffer, test_int);

        REQUIRE(buffer.size() == 4);
        REQUIRE(codec.decode(buffer) == test_int);
    }
}

TEST_CASE("Strings are encoded correctly", "[codec]") {
    const string_codec codec;
    bytebuf buffer;

    SECTION("Empty string") {
        std::string test_string = "";
        codec.encode(buffer, test_string);

        REQUIRE(buffer.size() == 1);
        REQUIRE(codec.decode(buffer) == test_string);
    }

    SECTION("Non-empty string") {
        std::string test_string = "Test string";
        codec.encode(buffer, test_string);

        REQUIRE(buffer.size() == 1 + 11);
        REQUIRE(codec.decode(buffer) == test_string);
    }
}