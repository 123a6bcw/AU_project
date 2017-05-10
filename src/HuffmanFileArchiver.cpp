#include "HuffmanFileArchiver.h"

HuffmanFileArchiver::HuffmanFileArchiver(): hios(){}
HuffmanFileArchiver::HuffmanFileArchiver(const HuffmanConstants::input_parametrs& parametrs): hios(parametrs.infile, parametrs.outfile), statistic(parametrs.statistic){}

static inline void clearFrequencies(HuffmanConstants::frequencies_t frequencies[HuffmanConstants::number_of_byte_values])
{
    memset(frequencies, 0, sizeof(HuffmanConstants::frequencies_t) * HuffmanConstants::number_of_byte_values);
}

void HuffmanFileArchiver::doTask(HuffmanConstants::HUFFMAN_TASK task)
{
    switch (task)
    {
        case HuffmanConstants::COMPRESS:
            compress();
            break;
        case HuffmanConstants::DECOMPRESS:
            decompress();
            break;
        case HuffmanConstants::NO_TASK:
            break;
        //may be others
    }
}

void HuffmanFileArchiver::compress()
{
    getFrequenciesFromCompressingFile();
    compressInputToOutput();
    if (statistic)
    {
        printStatistic(HuffmanConstants::COMPRESS);
    }
}

void HuffmanFileArchiver::decompress()
{
    getFrequenciesFromArchive();
    decompressInputToOutput();
    if (statistic)
    {
        printStatistic(HuffmanConstants::DECOMPRESS);
    }
}

void HuffmanFileArchiver::getFrequenciesFromCompressingFile()
{
    clearFrequencies(frequencies);
    uint8_t symbol = 0;
    while (true) //enumaretion of all byte values
    {
        hios.readInt(symbol);
        if (hios.inputEof())
        {
            break;
        }
        
        frequencies[symbol]++;
    }
    
    hios.reopenInput(); //data will be readed again
}

void HuffmanFileArchiver::writeFrequencies()
{
    uint16_t number_of_not_empty_symbols = 0;
    uint8_t symbol = 0;
    do //enumeration of all bytes values
    {
        if (frequencies[symbol] > 0)
        {
            number_of_not_empty_symbols++;
        }
        
        symbol++;
    } while (symbol != 0);
    
    hios.writeInt(number_of_not_empty_symbols);
    
    symbol = 0;
    do //enumeration of all bytes values
    {
        if (frequencies[symbol] > 0)
        {
            hios.writeInt(symbol);
            hios.writeInt(frequencies[symbol]);
        }
        
        symbol++;
    } while (symbol != 0);
}

void HuffmanFileArchiver::compressInputToOutput()
{
    HuffmanTree tree(frequencies);
    
    writeFrequencies();
    
    std::uint64_t number_of_bits_in_compressed_file = 0;
    uint8_t symbol = 0;
    do //enumeration of all byte values
    {
        if (frequencies[symbol] > 0)
        {
            number_of_bits_in_compressed_file += frequencies[symbol] * tree.getCode(symbol).size();
        }
        
        symbol++;
    } while (symbol != 0);
    
    hios.writeInt(number_of_bits_in_compressed_file);
    
    symbol = 0;
    while (true)
    {
        hios.readByte(symbol);
        if (hios.inputEof())
        {
            break;
        }
        
        hios.writeCode(tree.getCode(symbol));
    }
    
    hios.bitFlush();
    hios.flush();
}

void HuffmanFileArchiver::getFrequenciesFromArchive()
{
    clearFrequencies(frequencies);
    uint8_t symbol = 0;
    uint16_t number_of_symbols;
    hios.readInt(number_of_symbols);
    
    for (int i = 0; i < number_of_symbols; i++)
    {
        hios.readInt(symbol);  
        hios.readInt(frequencies[symbol]);
    }
}

void HuffmanFileArchiver::decompressInputToOutput()
{
    HuffmanTree tree(frequencies);
    HuffmanTree::HuffmanTreeWalker walker(tree); //class for iterating over the tree.
    
    uint64_t number_of_bits_in_compressed_file = 0;
    hios.readInt(number_of_bits_in_compressed_file); //last few bits in the file could be useless. Knowing total number of bits helps to avoid it  
    uint8_t current_byte = 0;
    
    while (true) //stop condition at the end
    {
        hios.readByte(current_byte);
        uint8_t meaningful_bits = std::min(HuffmanConstants::bits_in_byte, number_of_bits_in_compressed_file); //so it's 8 or less for the last byte in file
        
        for (std::size_t i = 0; i < meaningful_bits; i++)
        {
            bool current_bit = (current_byte & (1 << i));
            walker.walk(current_bit);
            
            if (walker.isLeaf())
            {
                hios.writeByte(walker.getSymbol());
            }
        }
        
        if (number_of_bits_in_compressed_file <= HuffmanConstants::bits_in_byte)
        {
            //that means there is not more then 8 bits left, and we just watched it at the last byte
            break;
        }
        
        number_of_bits_in_compressed_file -= HuffmanConstants::bits_in_byte; //this variable shows how many bits left at archived file
    }
    
    hios.flush();
}

void HuffmanFileArchiver::printStatistic(HuffmanConstants::HUFFMAN_TASK task)
{
    #include <stdio.h>
    std::size_t total_number_of_symbols = 0;
    std::size_t number_of_not_empty_symbols = 0;
    uint8_t symbol = 0;
    do
    {
        total_number_of_symbols += frequencies[symbol];
        
        if (frequencies[symbol] > 0)
        {
            number_of_not_empty_symbols++;
        }
        
        symbol++;
    } while (symbol != 0);
    
    std::size_t input_size = total_number_of_symbols;
    std::size_t additional_info_size = 2 + number_of_not_empty_symbols * 9 + 8;
    
    std::size_t archive_size = 0;
    switch (task)
    {
        case HuffmanConstants::COMPRESS:
            archive_size = (std::size_t)hios.outputTellp() - additional_info_size;
            printf("Архивирование:\nРазмер входных данных: %lu\nРазмер сжатых данных: %lu\nРазмер дополнительной информации: %lu\n", input_size, archive_size, additional_info_size);
            break;
        case HuffmanConstants::DECOMPRESS:
            hios.inputClear();
            archive_size = (std::size_t)hios.inputTellg() - additional_info_size;
            printf("Разархивирование:\nРазмер сжатых данных: %lu\nРазмер входных данных: %lu\nРазмер дополнительной информации: %lu\n", archive_size, input_size, additional_info_size);
            break;
        default:
            break;
    }
}

HuffmanConstants::frequencies_t HuffmanFileArchiver::getFrequency(uint8_t h)
{
    return frequencies[h];
}

void HuffmanFileArchiver::closeFiles()
{
    hios.closeFiles();
}
