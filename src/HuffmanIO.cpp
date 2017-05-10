#include "HuffmanIO.h"

HuffmanIO::HuffmanIO(){}

HuffmanIO::HuffmanIO(const std::string& inp, const std::string& outp)
{
    input.open(inp.c_str(), std::ifstream::in | std::ifstream::binary);
    
    if (!input.is_open())
    {
        throw std::runtime_error("Error opening input file");
    }
    
    output.open(outp.c_str(), std::ofstream::out | std::ofstream::binary);
    
    if (!output.is_open())
    {
        throw std::runtime_error("Error opening output file");
    }
}

void HuffmanIO::writeCode(const HuffmanConstants::packedBits_t& code)
{
    for (bool i : code)
    {
        writeBit(i);
    }
}

void HuffmanIO::bitFlush()
{
    if (bit_buffer_position > 0)
    {
        writeInt(bit_buffer);
        bit_buffer_position = 0;
    }
}

void HuffmanIO::flush()
{
    if (output_buffer_position > 0)
    {
		output.write((char*)output_buffer, output_buffer_position);
		output_buffer_position = 0;
		
		if (output.fail())
		{
		    throw std::runtime_error("The output file is damaged");
		}
    }
}

bool HuffmanIO::inputEof() const
{
    return input_buffer_length == 0;
    //what means we tryed to read symbol, input buffer has already readed all information from file and input_buffer_position reached input_buffer_length, so
    //it tryed to read more information from file, failed and give 0 as his length
}

void HuffmanIO::reopenInput()
{
    input.clear();
    input.seekg(std::ios_base::beg);
    input_buffer_length = 0;
    input_buffer_position = 0;
}

int64_t HuffmanIO::outputTellp()
{
    return output.tellp();
}

int64_t HuffmanIO::inputTellg()
{
    return input.tellg();
}

void HuffmanIO::inputClear()
{
    input.clear();
}

HuffmanIO::~HuffmanIO()
{
    bitFlush();
    flush();
}

void HuffmanIO::closeFiles()
{
    input.close();
    output.close();
}
