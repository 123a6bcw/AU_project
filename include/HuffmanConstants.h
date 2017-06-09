#ifndef MY_HUFFMAN_
#define MY_HUFFMAN_

#include <vector>
#include <string>

namespace HuffmanConstants
{
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
