#pragma once
#ifndef HAFFMANTREE_HPP
#define HAFFMANTREE_HPP

#include <unordered_map>
#include <queue>
#include <string>
#include <functional>
#include "interface.hpp"

struct HaffmanNode {
    unsigned char symbol;
    size_t frequency;
    HaffmanNode *left;
    HaffmanNode *right;

    HaffmanNode(unsigned char symbol, size_t frequency, HaffmanNode *left, HaffmanNode *right) :
        symbol{symbol}, frequency{frequency}, left{left}, right{right} {};

    bool isLeaf() {return left == nullptr && right == nullptr;}
};

struct HaffmanNodeComparator {
    bool operator()(HaffmanNode *left, HaffmanNode *right) {
        return left->frequency > right->frequency;
    }
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
    void InsertRoot(HaffmanNode *node);
    void printCodes() const;
    void visualiseTree() const;
    std::string getCode(unsigned char symbol) const;
    HaffmanNode* getRoot() const {return root;};
    unsigned getCodesSize() const {return codes.size();}

};

#endif // HAFFMANTREE_HPP