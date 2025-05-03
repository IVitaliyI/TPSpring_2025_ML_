#include "BitWriter.hpp"
#include <CharCounting.hpp>
#include "HaffmanTree.hpp"
#include "HaffmanEncAndDec.hpp"

#include <string>

int main() {
    // BitWriter bw;
    // bw.WriteByte(0xAA);
    // bw.WriteBit(1);
    // bw.WriteBit(1);
    // bw.WriteBit(0);
    // bw.WriteBit(1);
    // bw.WriteBit(0);
    // bw.WriteBit(1);
    // bw.visualizeBuffer(bw.GetBuffer());

    // CharCounting CharCount;
    // std::string str {"abracadabra"};
    // CharCount.CalculateCounting(str);
    // CharCount.visualizeCounting(CharCount.GetDictionary());

    // HaffmanTree tree;
    // tree.build(CharCount.GetDictionary());
    // tree.visualiseTree();
    // tree.printCodes();

    HaffmanEncAndDec hdec;
    // hdec.DecodeHaffmanTree(&tree);
    std::string input_filename{"../input_decode.txt"};
    std::string output_filename{"../output_decode.txt"};
    std::string output_final{"../output_final.txt"};
    {
    FileInputStream input(input_filename);
    FileOutputStream output(output_filename);

    hdec.Encode(input, output);}
    

    {
    FileInputStream input_final(output_filename);
    FileOutputStream output_fin(output_final);

    hdec.Decode(input_final, output_fin);
    }

    return 0;
}