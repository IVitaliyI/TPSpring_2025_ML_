#include <bitset>

#include "BitWriter.hpp"
#include "CharCounting.hpp"
#include "HaffmanTree.hpp"
#include "HaffmanEncAndDec.hpp"

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
        minHeap.push(new HaffmanNode('\0', first_min->frequency + second_min->frequency, first_min, second_min));
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
            HaffmanNode* leaf = new HaffmanNode{symbol, 0, nullptr, nullptr};
            nodeStack.push(leaf);
        } else {
            if (nodeStack.size() < 2) {
                break;
            }
            HaffmanNode *right = nodeStack.top(); nodeStack.pop();
            HaffmanNode *left = nodeStack.top(); nodeStack.pop();
            HaffmanNode *internal = new HaffmanNode{0, 0, left, right};
            nodeStack.push(internal);
            // std::cout << nodeStack.size() << std::endl;
        }
    }

    if (nodeStack.size() != 1) {
        throw std::runtime_error("Invalid tree encoding: more than one root candidate");
    }

    root = nodeStack.top();
    nodeStack.pop();
    codes.clear();
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
    unsigned char countNodes = tree->getCodesSize();
    unsigned char treeSize = countNodes * 10 - 1;
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
    for (int i = 0; i < 3; ++i) {
        if (!ReadBit(bit)) {
            throw std::runtime_error("Ошибка при чтении битов.");
        }
        result = (result << 1) | bit;
    }
    // std::cerr << "After 12 bits. Read: " << bits_read << " Byte Pos:" << byte_pos << " Bits left: " << bits_left << std::endl;
    return result;
}


void HaffmanEncAndDec::visualise_read_buffer() {
    for (auto &b : bitstring) {
        std::cout << std::bitset<8>{b} << "|";
    }

    std::cout << std::endl;
    unsigned length = bit_reader_buffer.ReadFirst12Bits();
    std::cout << "Length tree = " << length << std::endl;
    std::vector<byte> res = bit_reader_buffer.ReadTreeBits(length);
    for (auto &b : res) {
        std::cout << std::bitset<8>{b} << "|";
    }
    std::cout << std::endl;

    HaffmanTree read_tree;
    read_tree.build(res, length);
    read_tree.visualiseTree();
    read_tree.printCodes();

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

void HaffmanEncAndDec::Encode(IInputStream& original, IOutputStream& compressed) {
    HaffmanEncAndDec Encoder;
    Encoder.InitFromStream(original);
    CharCounting countingChars;
    countingChars.CalculateCounting(Encoder.bitstring);

    HaffmanTree tree;
    tree.build(countingChars.GetDictionary());

    size_t total_bits = 0;
    for (const auto& [symbol, frequency] : countingChars.GetDictionary()) {
        std::string code = tree.getCode(symbol);
        total_bits += frequency * code.length();
    }

    unsigned valid_bits_in_last_byte = total_bits % 8;
    if (valid_bits_in_last_byte == 0) valid_bits_in_last_byte = 8;

    for (int i = 2; i >= 0; --i) {
        bool bit = (valid_bits_in_last_byte >> i) & 1;
        Encoder.bit_writer_buffer.WriteBit(bit);
    }

    Encoder.EncodeHaffmanTree(&tree);
    // Encoder.bit_writer_buffer.visualizeBuffer(Encoder.bit_writer_buffer.GetBuffer());

    for (byte b : Encoder.bitstring) {
        const std::string &code = tree.getCode(b);
        for (char bit : code) {
            Encoder.bit_writer_buffer.WriteBit(bit - '0');
        }
    }
    Encoder.bit_writer_buffer.visualizeBuffer(Encoder.bit_writer_buffer.GetBuffer());

    Encoder.bit_writer_buffer.SaveBufferFile(compressed);

}


// void HaffmanEncAndDec::Decode(IInputStream& compressed, IOutputStream& original) {
//     HaffmanEncAndDec Decoder;
//     Decoder.InitFromStream(compressed);

//     // Decoder.visualize_read_buffer();
//     for (auto &s : Decoder.bitstring) {
//         std::cout << std::bitset<8>{s} << "|";
//     }
//     std::cout << std::endl;

//     BitReader reader{Decoder.bitstring, Decoder.bitstring.size() * 8};

//     unsigned valid_bits_in_last_byte = reader.ReadLastBitsCount();
//     std::cout << valid_bits_in_last_byte << std::endl;

//     HaffmanTree tree;
//     unsigned length = reader.ReadFirst12Bits();
//     // std::cout << "length = " << length << std::endl;
    
//     std::vector<byte> treeBits = reader.ReadTreeBits(length);

//     // for (auto &s : treeBits) {
//     //     std::cout << std::bitset<8>{s} << "|";
//     // }
//     // std::cout << std::endl;

//     tree.build(treeBits, length);
//     tree.printCodes();

//     byte b;
//     HaffmanNode* node = tree.getRoot();

//     while (reader.ReadByte(b)) {
//         int bits_to_read = (reader.byte_pos == reader.length - 1) ? valid_bits_in_last_byte : 8;
        
//         for (int i = bits_to_read - 1; i >= 0; --i) {
//             bool bit = (b >> i) & 1;
//             node = bit ? node->right : node->left;

//             if (!node) {
//                 throw std::runtime_error("Decode error: reached null node");
//             }

//             if (node->isLeaf()) {
//                 original.Write(node->symbol);
//                 node = tree.getRoot();
//             }
//         }

//         // Если это последний байт, проверяем, все ли биты были прочитаны
//         if (reader.byte_pos == reader.length - 1 && reader.bits_left == valid_bits_in_last_byte) break;
//     }

// }


void HaffmanEncAndDec::Decode(IInputStream& compressed, IOutputStream& original) {
    HaffmanEncAndDec Decoder;
    Decoder.InitFromStream(compressed);

    BitReader buffer{Decoder.bitstring, 3};

    unsigned valid_bits_in_last_byte = buffer.ReadLastBitsCount();
    std::cout << valid_bits_in_last_byte << std::endl;

    BitReader reader{Decoder.bitstring, (Decoder.bitstring.size() - 1) * 8 + valid_bits_in_last_byte};

    valid_bits_in_last_byte = reader.ReadLastBitsCount();

    HaffmanTree tree;
    unsigned length = reader.ReadFirst12Bits();

    std::vector<byte> treeBits = reader.ReadTreeBits(length);

    tree.build(treeBits, length);
    tree.printCodes();

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


