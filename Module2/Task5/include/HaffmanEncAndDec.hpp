#pragma once
#ifndef HAFFMANENCANDDEC_HPP
#define HAFFMANENCANDDEC_HPP

#include <bitset>
#include <cassert>
#include <fstream>
#include <stack>

#include "BitWriter.hpp"
#include "BitReader.hpp"
#include "HaffmanTree.hpp"
#include "interface.hpp"

class FileInputStream : public IInputStream {
private:    
    std::ifstream file;
public:
    FileInputStream(const std::string &filename) : file{filename, std::ios::binary} {
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
    };

    bool Read(byte &value) override {
        char c;
        if (file.get(c)) {
            value = c;
            return true;
        }
        return false;
    }
};

class FileOutputStream : public IOutputStream {
    private:
        std::ofstream file;
    public:
        FileOutputStream(const std::string &filename)
            : file{filename, std::ios::binary} {
            if (!file.is_open()) {
                throw std::runtime_error("Cannot open file for writing: " + filename);
            }
        }
    
        void Write(byte value) override {
            if (!file.put(value)) {
                throw std::runtime_error("Failed to write byte to file.");
            }
            file.flush();

        }
    
        ~FileOutputStream() {
            file.close();
        }
};
    

class HaffmanEncAndDec
{
private:
    BitWriter bit_writer_buffer;
    BitReader bit_reader_buffer;
    std::vector<byte> bitstring;
    void EncodeHaffmanTreeInherit(HaffmanNode *node);
    void ReadAllFromStream(IInputStream& stream);
public:        
    
    void EncodeHaffmanTree(HaffmanTree *tree);
    HaffmanEncAndDec() : bit_reader_buffer(bitstring, 0) {};
    void InitFromFile(const std::string &filename) {
        FileInputStream file_input(filename);
        ReadAllFromStream(file_input);
    }

    void InitFromStream(IInputStream &input) {
        ReadAllFromStream(input);
    }

    void visualise_read_buffer();
    // ~HaffmanEncAndDec();

    static void Encode(IInputStream& original, IOutputStream& compressed);
    static void Decode(IInputStream& compressed, IOutputStream& original);
};



#endif // HAFFMANENCANDDEC_HPP