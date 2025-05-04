#define TEST_SYS true

#if TEST_SYS
#include "Huffman.h"
#endif
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <bitset>
#include <cassert>
#include <stack>
#include <unordered_map>
#include <queue>
#include <string>
#include <functional>

#if !TEST_SYS
#include "interface.hpp"
#endif
#define DEBUG false

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
    size_t getBitsRead() const { return bits_read; }
    std::vector<byte> ReadTreeBits(unsigned count);
    size_t bits_left = 0;
    size_t length;
    size_t byte_pos = 0;

};

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



class CharCounting {
    private:
        std::unordered_map<unsigned char, size_t> dictionary;
    public:
        CharCounting() = default;
        ~CharCounting() = default;
        void CalculateCounting(std::vector<byte> &str);
        const std::unordered_map<unsigned char, size_t>& GetDictionary() const;
        void visualizeCounting(const std::unordered_map<unsigned char, size_t>& dict) const;
        
};


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
    
struct HaffmanNode {
    unsigned char symbol;
    size_t frequency;
    HaffmanNode *left;
    HaffmanNode *right;

    HaffmanNode(unsigned char symbol, size_t frequency, HaffmanNode *left, HaffmanNode *right) :
        symbol{symbol}, frequency{frequency}, left{left}, right{right} {};

    bool isLeaf() {return left == nullptr && right == nullptr;}
};


class HaffmanTree
{
private:
    HaffmanNode *root;
    std::unordered_map<unsigned char, std::string> codes;

    void destroy(HaffmanNode *node);
    void generateCodes(HaffmanNode *node, const std::string &prefix);
    void deleteTree(HaffmanNode* node);


public:
    HaffmanTree(/* args */);
    HaffmanTree(HaffmanNode *node) {root = node;};
    ~HaffmanTree();

    void build(const std::unordered_map<unsigned char, size_t> &dictionary);
    void build(std::vector<byte> bitstring, unsigned count);
    void build(BitReader &reader, unsigned count);

    void InsertRoot(HaffmanNode *node);
    void printCodes() const;
    void visualiseTree() const;
    std::string getCode(unsigned char symbol) const;
    HaffmanNode* getRoot() const {return root;};
    unsigned getCodesSize() const {return codes.size();}

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

    static void EncodeInter(IInputStream& original, IOutputStream& compressed);
    static void DecodeInter(IInputStream& compressed, IOutputStream& original);
};





struct HaffmanNodeComparator {
    bool operator()(HaffmanNode *left, HaffmanNode *right) {
        return left->frequency > right->frequency;
    }
};

BitWriter::BitWriter() : bitCount(0) {};

void BitWriter::WriteBit(unsigned char bit) {
    if (bitCount % 8 == 0) {
        buffer.push_back(0u);
    }
    if (bit)
        buffer[bitCount / 8] |= 1 << (7 - bitCount % 8);
    bitCount++; 
}

void BitWriter::WriteByte(unsigned char byte) {
    if (bitCount % 8 == 0) {
        buffer.push_back(byte);
    } else {
        unsigned offset = bitCount % 8;
        buffer[bitCount / 8] |= byte >> offset;
        buffer.push_back(byte << (8 - offset)); 
    }
    bitCount += 8;
}

const std::vector<unsigned char>& BitWriter::GetBuffer() const {
    return buffer;
}

size_t BitWriter::GetBitCount() const {
    return bitCount;
}

void BitWriter::visualizeBuffer(const std::vector<unsigned char> &buffer) const {
    for (auto &b: buffer) {
        std::cout << std::bitset<8>(b) << "|";
    }
    std::cout << std::endl;
}

void CharCounting::CalculateCounting(std::vector<byte> &str) {
     dictionary.clear();
    for (auto s: str) {
        dictionary[s]++;
    }
}

const std::unordered_map<unsigned char, size_t>& CharCounting::GetDictionary() const {
    return dictionary;
}

void CharCounting::visualizeCounting(const std::unordered_map<unsigned char, size_t>& dict) const {
    for (const auto& n : dict) {
        std::cout << "Char = [" << n.first << "]. Char count = " << n.second << std::endl;
    }
}

// HaffmanTree methods
void HaffmanTree::build(const std::unordered_map<unsigned char, size_t> &dictionary) {
    std::priority_queue<HaffmanNode*, std::vector<HaffmanNode*>, HaffmanNodeComparator> minHeap;
    HaffmanNode *first_min, *second_min;
    std::string buf = "";
    for (auto [sym, freq] : dictionary) {
        minHeap.push(new HaffmanNode(sym, freq, nullptr, nullptr));
    }

    while (minHeap.size() > 1) {
        first_min = minHeap.top();
        minHeap.pop();
        second_min = minHeap.top();
        minHeap.pop();
        minHeap.push(new HaffmanNode(0, first_min->frequency + second_min->frequency, first_min, second_min));
    }
    root = !minHeap.empty() ? minHeap.top() : nullptr;
    generateCodes(root, buf);
}


void HaffmanTree::build(std::vector<byte> bitstring, unsigned count) {
    BitReader reader(bitstring, count);
    std::stack<HaffmanNode*> nodeStack;
    bool bit;

    while (reader.ReadBit(bit)) {
        if (bit) {
            byte symbol;
            if (!reader.ReadByte(symbol)) {
                throw std::runtime_error("Unexpected end of stream while reading symbol");
            }
            nodeStack.push(new HaffmanNode{symbol, 0, nullptr, nullptr});
        } else {
            if (nodeStack.size() < 2) {
                throw std::runtime_error("Invalid tree: stack underflow");
            }
            auto right = nodeStack.top(); nodeStack.pop();
            auto left = nodeStack.top(); nodeStack.pop();
            nodeStack.push(new HaffmanNode{0, 0, left, right});
        }
    }

    if (nodeStack.size() != 1) {
        throw std::runtime_error("Invalid tree: leftover nodes");
    }

    root = nodeStack.top();
    nodeStack.pop();
    codes.clear();
    generateCodes(root, "");
}

void HaffmanTree::build(BitReader &reader, unsigned count) {
    std::stack<HaffmanNode*> stack;
    unsigned bits = 0;
    bool bit;
    while (bits < count && reader.ReadBit(bit)) {
        bits++;
        if (bit) {
            byte symbol = 0;
            for (int i = 0; i < 8; ++i) { reader.ReadBit(bit); symbol = (symbol<<1)|bit; bits++; }
            stack.push(new HaffmanNode{symbol,0,nullptr,nullptr});
        } else {
            auto r = stack.top(); stack.pop();
            auto l = stack.top(); stack.pop();
            stack.push(new HaffmanNode{0,0,l,r});
        }
    }
    if (stack.size()!=1) throw std::runtime_error("Invalid tree");
    root = stack.top();
    generateCodes(root, "");
}



void HaffmanTree::destroy(HaffmanNode *node) {}
void HaffmanTree::generateCodes(HaffmanNode *node, const std::string &prefix) {
    if (node == nullptr) return;
    if (node->left == nullptr && node->right == nullptr) {
        codes[node->symbol] = prefix;
        return; 
    }
    generateCodes(node->left, prefix + "0");
    generateCodes(node->right, prefix + "1");
}

HaffmanTree::HaffmanTree() : root(nullptr) {}

HaffmanTree::~HaffmanTree() {
    deleteTree(root);
}

void HaffmanTree::deleteTree(HaffmanNode* node) {
    if (node == nullptr) return;

    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}


void HaffmanTree::printCodes() const {
    for (auto [sym, code] : codes) {
        std::cout << "Sym = " << sym << " Code = " << code << std::endl;
    }
}

void HaffmanTree::visualiseTree() const {
    if (!root) {
        std::cout << "Haffman tree is empty.\n";
        return;
    }

    std::queue<std::pair<const HaffmanNode*, std::string>> q;
    q.push({root, ""});

    while (!q.empty()) {
        auto [node, prefix] = q.front();
        q.pop();

        std::cout << prefix;
        if (!node->left && !node->right) {
            if (std::isprint(node->symbol))
                std::cout << "'" << node->symbol << "' ";
            else
                std::cout << "[" << static_cast<int>(node->symbol) << "] ";
        } else {
            std::cout << "* ";
        }

        std::cout << "(freq: " << node->frequency << ")\n";

        if (node->left)
            q.push({node->left, prefix + "  L─"});
        if (node->right)
            q.push({node->right, prefix + "  R─"});
    }
}

std::string HaffmanTree::getCode(unsigned char symbol) const {
    auto it = codes.find(symbol);
    if (it != codes.end()) {
        return it->second;
    }
    return "";
}

void HaffmanTree::InsertRoot(HaffmanNode *node) {
    if (root == nullptr) {
        root = node;
    }
}

void HaffmanEncAndDec::EncodeHaffmanTreeInherit(HaffmanNode *node) {
    if (!node) return;

    if (node->isLeaf()) {
        bit_writer_buffer.WriteBit(1);
        bit_writer_buffer.WriteByte(node->symbol);
    } else {
        EncodeHaffmanTreeInherit(node->left);
        EncodeHaffmanTreeInherit(node->right);
        bit_writer_buffer.WriteBit(0);
    }
}



void HaffmanEncAndDec::EncodeHaffmanTree(HaffmanTree *tree) {
    size_t countNodes = tree->getCodesSize();
    size_t treeSize = countNodes * 10 - 1;
    assert(treeSize <= 4095);
    std::bitset<12> length{treeSize};
    for (size_t i = 0; i < length.size(); ++i) {
        bit_writer_buffer.WriteBit(length[i]);
    }
    // std::cout << "Buffer size = " << bit_writer_buffer.GetBitCount() << std::endl;
    // bit_writer_buffer.visualizeBuffer(bit_writer_buffer.GetBuffer());
    EncodeHaffmanTreeInherit(tree->getRoot());
    

    // FileOutputStream output{"../binary_tree.bin"};
    // bit_writer_buffer.SaveBufferFile(output);
}

bool BitReader::ReadBit(bool &bit) {
    if (bits_read >= length) return false;  

    if (bits_left == 0) {
        if (byte_pos >= data.size()) return false;
        buffer = data[byte_pos++];
        bits_left = 8;
    }

    bit = (buffer >> (bits_left - 1)) & 1;
    bits_left--;
    bits_read++;
    return true;
}

bool BitReader::ReadByte(byte &value) {
    if (bits_left == 0) {
        if (byte_pos >= data.size()) return false;
        value = data[byte_pos++];
        bits_left = 8;
        return true;
    }
    
    value = 0;
    for (int i = 0; i < 8; i++) {
        bool b;
        if (!ReadBit(b)) return false;
        value = (value << 1) | b;
    }
    return true;
}

void HaffmanEncAndDec::ReadAllFromStream(IInputStream& stream) {
    
    bitstring.clear();
    byte value;
    
    size_t count = 0; 
    while (stream.Read(value)) {
        bitstring.push_back(value);
        count++;
    }
        
}

unsigned BitReader::ReadFirst12Bits() {
    unsigned result = 0;
    bool bit;
    for (int i = 0; i < 12; ++i) {
        if (!ReadBit(bit)) {
            throw std::runtime_error("Ошибка при чтении битов.");
        }
        result |= (bit << i);
    }
    // std::cerr << "After 12 bits. Read: " << bits_read << " Byte Pos:" << byte_pos << " Bits left: " << bits_left << std::endl;
    return result;
}

unsigned BitReader::ReadLastBitsCount() {
    unsigned result = 0;
    bool bit;
    for (int i = 2; i >= 0; --i) {
        if (!ReadBit(bit)) {
            throw std::runtime_error("Ошибка при чтении битов.");
        }
        result |= (bit << i);  // старший бит первым
    }
    
    return (result == 0) ? 8 : result;
}

std::vector<byte> BitReader::ReadTreeBits(unsigned count) {
    std::vector<byte> result;
    byte current_byte = 0;
    int bits_written = 0;

    for (unsigned i = 0; i < count; i++) {
        bool bit;

        if (!ReadBit(bit)) {
            throw std::runtime_error("Unexpected end of bitstream while reading tree bits");
        }
        // std::cerr << "After 12 bits. Read: " << bits_read << " Byte Pos:" << byte_pos << " Bits left: " << bits_left << " bit value: " << bit << std::endl;

        current_byte = (current_byte << 1) | bit;
        bits_written++;
        if (bits_written == 8) {
            result.push_back(current_byte);
            current_byte = 0;
            bits_written = 0;
        }
    }
    if (bits_written > 0) {
        current_byte = current_byte << (8 - bits_written);
        result.push_back(current_byte);
    }
    return result;
}


void HaffmanEncAndDec::EncodeInter(IInputStream& original, IOutputStream& compressed) {
    HaffmanEncAndDec Encoder;
    Encoder.InitFromStream(original);
    CharCounting countingChars;
    countingChars.CalculateCounting(Encoder.bitstring);

    HaffmanTree tree;
    tree.build(countingChars.GetDictionary());
#if DEBUG
    countingChars.visualizeCounting(countingChars.GetDictionary());
    tree.visualiseTree();
#endif
    unsigned total_bits_mod_8 = 0;
    for (const auto &[symbol, frequency] : countingChars.GetDictionary()) {
        unsigned code_len = tree.getCode(symbol).length();
        total_bits_mod_8 = (total_bits_mod_8 + (code_len * frequency) % 8) % 8;
    }

    unsigned valid_bits = (total_bits_mod_8 + 3 + (12 % 8) + (10 * tree.getCodesSize()) % 8 - 1) % 8;
    if (valid_bits == 0) valid_bits = 8;
#if DEBUG
    std::cout << "valid Bits " << valid_bits << std::endl; 
#endif
    for (int i = 2; i >= 0; i--) {
        bool bit = (valid_bits >> i) & 1u;
        Encoder.bit_writer_buffer.WriteBit(bit);
    }

    Encoder.EncodeHaffmanTree(&tree);
#if DEBUG
    Encoder.bit_writer_buffer.visualizeBuffer(Encoder.bit_writer_buffer.GetBuffer());
#endif

    for (byte b : Encoder.bitstring) {
        const std::string code = tree.getCode(b);
        for (auto c : code) {
            Encoder.bit_writer_buffer.WriteBit(c - '0');
        }
    }

#if DEBUG
    Encoder.bit_writer_buffer.visualizeBuffer(Encoder.bit_writer_buffer.GetBuffer());
#endif


    // HaffmanEncAndDec Encoder;
    // Encoder.InitFromStream(original);
    // CharCounting countingChars;
    // countingChars.CalculateCounting(Encoder.bitstring);

    // HaffmanTree tree;
    // tree.build(countingChars.GetDictionary());

    // size_t total_bits = 0;
    // for (const auto& [symbol, frequency] : countingChars.GetDictionary()) {
    //     std::string code = tree.getCode(symbol);
    //     total_bits += frequency * code.length();
    // }

    // unsigned valid_bits_in_last_byte = (total_bits + 3 + 12 + 10 * tree.getCodesSize() - 1) % 8;
    // if (valid_bits_in_last_byte == 0) valid_bits_in_last_byte = 8;

    // for (int i = 2; i >= 0; --i) {
    //     bool bit = (valid_bits_in_last_byte >> i) & 1;
    //     Encoder.bit_writer_buffer.WriteBit(bit);
    // }

    // Encoder.EncodeHaffmanTree(&tree);
    // // Encoder.bit_writer_buffer.visualizeBuffer(Encoder.bit_writer_buffer.GetBuffer());

    // for (byte b : Encoder.bitstring) {
    //     const std::string &code = tree.getCode(b);
    //     for (char bit : code) {
    //         Encoder.bit_writer_buffer.WriteBit(bit - '0');
    //     }
    // }
    // // Encoder.bit_writer_buffer.visualizeBuffer(Encoder.bit_writer_buffer.GetBuffer());

    Encoder.bit_writer_buffer.SaveBufferFile(compressed);

}


void HaffmanEncAndDec::DecodeInter(IInputStream& compressed, IOutputStream& original) {
    // HaffmanEncAndDec tmp;
    // tmp.InitFromStream(compressed);
    // auto &bits = tmp.bitstring;
    // size_t total_bits = bits.size() * 8ULL;

    // BitReader reader{bits, total_bits};

    // unsigned stored = 0;
    // for (int i = 3; i >= 0; --i) {
    //     bool b; reader.ReadBit(b);
    //     stored |= (b << i);
    // }
    // unsigned valid_bits_in_last_byte = stored == 0 ? 8 : stored;

    // unsigned tree_bits_count = reader.ReadFirst12Bits();

    // std::cout << "Tree bits^ " << tree_bits_count << std::endl; 

    // HaffmanTree tree;
    // tree.build(reader, tree_bits_count);

    // tree.visualiseTree();

    // HaffmanNode* node = tree.getRoot();
    // bool bit;
    // while (reader.ReadBit(bit)) {
    //     node = bit ? node->right : node->left;
    //     if (!node) throw std::runtime_error("Decode error: null node");
    //     if (node->isLeaf()) {
    //         original.Write(node->symbol);
    //         node = tree.getRoot();
    //     }
    // }

    HaffmanEncAndDec Decoder;
    Decoder.InitFromStream(compressed);

    BitReader buffer{Decoder.bitstring, 3};

    unsigned valid_bits_in_last_byte = buffer.ReadLastBitsCount();
#if DEBUG
    std::cout << valid_bits_in_last_byte << std::endl;
#endif

    BitReader reader{Decoder.bitstring, (Decoder.bitstring.size() - 1) * 8 + (valid_bits_in_last_byte)};

    valid_bits_in_last_byte = reader.ReadLastBitsCount();
#if DEBUG
    std::cout << valid_bits_in_last_byte << std::endl;
#endif
    HaffmanTree tree;

    unsigned tree_bits_count = reader.ReadFirst12Bits();
#if DEBUG
    std::cout << tree_bits_count << std::endl;
#endif
    tree.build(reader, tree_bits_count);
#if DEBUG
    tree.visualiseTree();

    tree.printCodes();
#endif

    byte b;
    HaffmanNode* node = tree.getRoot();
    bool bit;
    while (reader.ReadBit(bit)) {
        node = bit ? node->right : node->left;
    
        if (!node) {
            throw std::runtime_error("Decode error: reached null node");
        }
    
        if (node->isLeaf()) {
            original.Write(node->symbol);
            node = tree.getRoot();
        }
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) 
{
    HaffmanEncAndDec::EncodeInter(original, compressed);
}

void Decode(IInputStream& compressed, IOutputStream& original) 
{ 
    HaffmanEncAndDec::DecodeInter(compressed, original);
}

#if !TEST_SYS
int main() {
    std::string input_filename{"./input.txt"};
    std::string output_filename{"./encode.txt"};
    std::string output_final{"./output.txt"};
    {
    FileInputStream input(input_filename);
    FileOutputStream output(output_filename);

    Encode(input, output);}

    {
    FileInputStream input_final(output_filename);
    FileOutputStream output_fin(output_final);

    Decode(input_final, output_fin);
    }

    return 0;
}
#endif
