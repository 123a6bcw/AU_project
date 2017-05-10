#ifndef MY_HUFFMAN_ARC
#define MY_HUFFMAN_ARC

#include <stdint.h>
#include <string.h>
#include "HuffmanConstants.h"
#include "HuffmanTree.h"
#include "HuffmanIO.h"

class HuffmanFileArchiver
{
public:
    HuffmanFileArchiver();
    HuffmanFileArchiver(const HuffmanConstants::input_parametrs& parametrs);
    void doTask(HuffmanConstants::HUFFMAN_TASK task);
    void compress();
    void decompress();
    
    void closeFiles();
    HuffmanConstants::frequencies_t getFrequency(uint8_t h);
private:
    HuffmanIO hios;
    bool statistic;
    HuffmanConstants::frequencies_t frequencies[HuffmanConstants::number_of_byte_values];
    
    void getFrequenciesFromCompressingFile();
    void writeFrequencies();
    void compressInputToOutput();
    void getFrequenciesFromArchive();
    void decompressInputToOutput();
    void printStatistic(HuffmanConstants::HUFFMAN_TASK task);
};
#endif
