#pragma once
#ifndef BITREADER_HPP
#define BITREADER_HPP

#include <fstream>
#include <vector>
#include "interface.hpp"

class BitReader
{
private:
    const std::vector<byte> &data;
    size_t bits_read = 0;
    byte buffer = 0;
public:
    BitReader(const std::vector<byte> &data_ref, size_t length): data{data_ref}, length{length} {}
    bool ReadBit(bool &bit);
    bool ReadByte(byte &value);
    unsigned ReadFirst12Bits();
    unsigned ReadLastBitsCount();
    std::vector<byte> ReadTreeBits(unsigned count);
    size_t bits_left = 0;
    size_t length;
    size_t byte_pos = 0;

};

#endif // BITREADER_HPP