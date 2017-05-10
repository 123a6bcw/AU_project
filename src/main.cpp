#include "HuffmanFileArchiver.h"
#include <exception>

static void parseAgrumentsFromCommandLine(HuffmanConstants::HUFFMAN_TASK& task, HuffmanConstants::input_parametrs& parametrs, int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            task = HuffmanConstants::COMPRESS;
        } else
        if (strcmp(argv[i], "-u") == 0)
        {
            task = HuffmanConstants::DECOMPRESS;
        } else
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0)
        {
            i++; //next one isn't a key, so should be skipped;
            if (i < argc)
            {
                parametrs.infile = argv[i];
            }
        } else
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
        {
            i++;
            if (i < argc)
            {
                parametrs.outfile = argv[i];
            }
        } else
        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--stats") == 0)
        {
            parametrs.statistic = true;
        } else
        {
            throw std::invalid_argument(argv[i]);
        }
    }
    
    if (task == HuffmanConstants::NO_TASK)
    {
        throw std::logic_error("No given task for Huffman");
    }
}

int main (int argc, char* argv[])
{
    HuffmanConstants::HUFFMAN_TASK task = HuffmanConstants::NO_TASK;
    HuffmanConstants::input_parametrs parametrs;
    
    try
    {
        parseAgrumentsFromCommandLine(task, parametrs, argc, argv);
    }
    
    catch (std::invalid_argument& e)
    {
        printf("unknown key: %s\n", e.what());
        return 0;
    }
    
    try
    {
        HuffmanFileArchiver archiver(parametrs);
        archiver.doTask(task);
    }
    
    catch (std::exception& e)
    {
        printf("%s\n", e.what());
        return 0;
    }
}
