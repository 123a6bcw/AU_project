#ifndef MY_HUFFMAN_
#define MY_HUFFMAN_

#include <vector>
#include <string>

namespace HuffmanConstants
{   
    /*
        Archive format:
            1) 2 bytes for number of symbols in input file
            2) for each symbol 1 byte for value and 8 bytes for it's frequency
            3) 8 bytes for number of bits in compressed data
            4) Compressed data
    */

    const std::size_t number_of_byte_values = 256;
    const std::size_t bits_in_byte = 8;
    typedef std::vector<bool> packedBits_t;
        //Set of bits packed into vector
    typedef uint64_t frequencies_t;
        //in case I'll want to change it to uint32_t. 
    enum HUFFMAN_TASK {NO_TASK, COMPRESS, DECOMPRESS}; //may be others
    
    struct input_parametrs
    {
        std::string infile;
        std::string outfile;
        bool statistic = false;
        
        input_parametrs(){}
        input_parametrs(std::string inf, std::string outf, bool stat = false): infile(inf), outfile(outf), statistic(stat){}
    };
}

#endif
