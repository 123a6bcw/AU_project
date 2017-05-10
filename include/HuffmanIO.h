#ifndef HUFFMAN_IOS
#define HUFFMAN_IOS

#include <utility>
#include <stdint.h>
#include <fstream>
#include <exception>
#include "HuffmanConstants.h"

class HuffmanIO
{
friend class HuffmanFileArchiver;
public:
    HuffmanIO();
    HuffmanIO(const std::string& inp, const std::string& outp);
    
    ~HuffmanIO();
    template <class T>
    inline void readInt(T& a); //all inline function defined in this file
    
    template <class T>
    inline void writeInt(const T& a);
    
    inline void readByte(uint8_t& s);
    inline void writeByte(const uint8_t& s);
    inline void writeBit(const bool& b);
    
    void writeCode(const HuffmanConstants::packedBits_t& inf);
    bool inputEof() const;
    void reopenInput();
    int64_t outputTellp();
    int64_t inputTellg();
    
    void bitFlush();
    void flush();
    
    void openInput(const std::string& inp);
    void openOutput(const std::string& out);
    
    void closeFiles();
private:
    std::ifstream input;
    std::ofstream output;
    
    const static uint32_t input_buffer_size = 4096;
    uint32_t input_buffer_length = 0; //that means input_buffer is empty
    uint32_t input_buffer_position = 0;
    uint8_t input_buffer[input_buffer_size];
    
    const static uint32_t output_buffer_size = 4096;
    uint32_t output_buffer_position = 0; //that means output_buffer is empty
    uint8_t output_buffer[output_buffer_size];
    
    uint8_t bit_buffer = 0;
    uint8_t bit_buffer_position = 0; //for bit_buffer this isn't important
    
    inline void checkInputBufferOverflow();
    inline void checkOutputBufferOverflow();
    inline void checkBitOutputOverflow();
    void inputClear();
    
    HuffmanIO(HuffmanIO& other);
    HuffmanIO& operator=(HuffmanIO& other); //banned cause of rule of three.
};

template <class T>
inline void HuffmanIO::readInt(T& a) //defined here so the chance of being inline will be higher
{
    for (int8_t i = sizeof(T) - 1; i >= 0; i--) //the highest byte should come first.
    {                                           //sizeof(T) > 0 (no overflow)
        readByte(*((uint8_t*)(&a) + i)); //consecutively read every byte in IntType
    }
}

template <class T>
inline void HuffmanIO::writeInt(const T& a)
{
    for (int8_t i = sizeof(T) - 1; i >= 0; i--) 
    {
        writeByte(*((const uint8_t*)(&a) + i));
    }
}

inline void HuffmanIO::checkInputBufferOverflow()
{
    if (input_buffer_position == input_buffer_length)
    {
	    input_buffer_position = 0;
	    input.read((char*) input_buffer, input_buffer_size); 
	    input_buffer_length = input.gcount(); //at the end of the file could be less bytes than input_buffer_size
	    
	    if (!input.eof() && input.fail())
	    {
	        throw std::runtime_error("Input file is damaged");
        }
    }
}

inline void HuffmanIO::readByte(uint8_t& s)
{
    checkInputBufferOverflow();
    s = input_buffer[input_buffer_position++];
}

inline void HuffmanIO::checkOutputBufferOverflow()
{
    if (output_buffer_position == output_buffer_size)
    {
        output_buffer_position = 0;
	    output.write((char*) output_buffer, output_buffer_size);
	    
	    if (output.fail())
	    {
	        throw std::runtime_error("Output file is damaged");
        }
	}
}

inline void HuffmanIO::writeByte(const uint8_t& s)
{
    checkOutputBufferOverflow();
    output_buffer[output_buffer_position++] = s;
}

inline void HuffmanIO::checkBitOutputOverflow()
{
    if (bit_buffer_position == HuffmanConstants::bits_in_byte)
    {     
        writeInt(bit_buffer);    
        bit_buffer = 0;
        bit_buffer_position = 0;
    }
}

inline void HuffmanIO::writeBit(const bool& b)
{
    checkBitOutputOverflow();
    bit_buffer |= (b << (bit_buffer_position++));
}

#endif
