#pragma once
#ifndef CHARCOUNTING_HPP
#define CHARCOUNTING_HPP

#include <unordered_map>
#include <string>

#include "interface.hpp"
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

#endif // CHARCOUNTING_HPP