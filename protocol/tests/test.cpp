#include <catch2/catch_test_macros.hpp>

#include "catch2/generators/catch_generators.hpp"
#include "catch2/generators/catch_generators_range.hpp"

#include "protocol/codec/string_codec.h"
#include "protocol/bytebuf/bytebuf.h"
#include "protocol/codec/uint32_codec.h"
#include "protocol/codec/uint8_codec.h"

TEST_CASE("UINT8 is encoded correctly") {
    uint8_codec codec;
    bytebuf buffer;

    SECTION("Valid UINT8") {
        uint8_t test_int = GENERATE(range(0, 255));
        codec.encode(buffer, test_int);

        REQUIRE(buffer.size() == 1);
        REQUIRE(codec.decode(buffer) == test_int);
    }
}

TEST_CASE("UINT32 is encoded correctly") {
    uint32_codec codec;
    bytebuf buffer;

    SECTION("Valid UINT32") {
        uint32_t test_int = GENERATE(range(0, 1023));
        codec.encode(buffer, test_int);

        REQUIRE(buffer.size() == 4);
        REQUIRE(codec.decode(buffer) == test_int);
    }
}

TEST_CASE("Strings are encoded correctly", "[codec]") {
    string_codec codec;
    bytebuf buffer;

    SECTION("Empty string") {
        std::string test_string = "";
        codec.encode(buffer, test_string);

        REQUIRE(codec.decode(buffer) == test_string);
    }

    SECTION("Non-empty string") {
        std::string test_string = "Test string";
        codec.encode(buffer, test_string);

        REQUIRE(codec.decode(buffer) == test_string);
    }
}