// #define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "BitWriter.hpp"

TEST_CASE("WriteBit simple") {
    BitWriter bw;
    bw.WriteBit(1);
    bw.WriteBit(0);
    bw.WriteBit(1);
    bw.WriteBit(1);
    bw.WriteBit(0);
    bw.WriteBit(0);
    bw.WriteBit(0);
    bw.WriteBit(1);

    auto buffer = bw.GetBuffer();
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer[0] == 0b10110001);
    REQUIRE(bw.GetBitCount() == 8);
}

TEST_CASE("WriteByte aligned") {
    BitWriter bw;
    bw.WriteByte(0xAA);

    auto buffer = bw.GetBuffer();
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer[0] == 0xAA);
    REQUIRE(bw.GetBitCount() == 8);
}

TEST_CASE("Write two Byte and 3 bits") {
    BitWriter bw;
    bw.WriteBit(1);
    bw.WriteByte(0xAA);
    bw.WriteByte(0xBB);
    bw.WriteBit(0);
    bw.WriteBit(1);

    const auto& buffer = bw.GetBuffer();
    size_t bitCount = bw.GetBitCount();

    REQUIRE(bitCount == 1 + 8 + 8 + 1 + 1);

    REQUIRE(buffer.size() == (bitCount + 7) / 8);

    REQUIRE(buffer[0] == 0b11010101);
    REQUIRE(buffer[1] == 0b01011101);
    REQUIRE(buffer[2] == 0b10100000); 
}