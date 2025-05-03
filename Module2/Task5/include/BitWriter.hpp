#pragma once
#ifndef BITWRITER_HPP
#define BITWRITER_HPP

#include <iostream>
#include <vector>
#include <bitset>
#include "interface.hpp"

class BitWriter {
public:

    BitWriter();

    void WriteBit(unsigned char bit);

    void WriteByte(unsigned char byte);

    const std::vector<unsigned char>& GetBuffer() const;

    size_t GetBitCount() const;

    void visualizeBuffer(const std::vector<unsigned char> &buffer) const;

    void SaveBufferFile(IOutputStream &output) {
        for (auto &b : buffer) {
            // std::cout << "WRITE :" << "|" << std::bitset<8>{b} << "|" << std::endl;
            output.Write(b);
        }
    }
    

private:
    std::vector<unsigned char> buffer;
    size_t bitCount;
};

#endif // BITWRITER_HPP


