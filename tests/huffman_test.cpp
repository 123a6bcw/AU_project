#include <gtest/gtest.h>
#include "HuffmanFileArchiver.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>

using namespace testing;
using namespace std;

TEST(HuffmanIO_test, reading_test)
{
    ofstream output("./tests/testfiles/tmp");
    output.put(5);
    output.put(7);
    
    for (int i = 0; i < 8; i++)
        output.put((char)255);
    
    output.put('\0');
    
    for (int i = 0; i < 4096 * 2 + 5; i++)
    {
        output.put((char) i);
    }
    
    output.close();
    
    
    string inp = "./tests/testfiles/tmp";
    string outp = "./tests/testfiles/tmp2";
    
    HuffmanIO hios(inp, outp);
    
    uint8_t h;
    hios.readInt(h);
    EXPECT_TRUE(h == 5);
    hios.readByte(h);
    EXPECT_TRUE(h == 7);
    
    uint64_t number;
    hios.readInt(number);
    EXPECT_TRUE(number == (uint64_t)(-1));
    
    hios.readByte(h);
    EXPECT_TRUE(h == '\0');
    
    for (int i = 0; i < 4096 * 2 + 5; i++)
    {
        hios.readInt(h);
        EXPECT_TRUE(h == (uint8_t) i);
    }
    
    hios.readByte(h);
    EXPECT_TRUE(hios.inputEof());
}

TEST(HuffmanIO_test, writing_test)
{
    string inp = "./tests/testfiles/tmp";
    string outp = "./tests/testfiles/tmp2";
    HuffmanIO hios(inp, outp);
    
    hios.writeInt((uint64_t)(-1));
    hios.writeByte('\0');
    
    for (int i = 0; i < 4096 + 5; i++)
    {
        hios.writeInt((uint8_t)i);
    }
    
    //NOTE: writing DOES NOT working if there if writeBit before writeInt/Byte. But that's alright, it never happens while archiving
    for (int i = 0; i < 4096 + 5; i++)
    {
        hios.writeBit(1);
        for (int g = 0; g < 5; g++)
            hios.writeBit(0);
        hios.writeBit(1);
        hios.writeBit(0);
    }
    
    vector<bool> pp;
    for (int i = 0; i < 8; i++)
        pp.push_back(1);
    for (int i = 0; i < 8; i++)
        pp.push_back(1);
    for (int i = 0; i < 2; i++)
        pp.push_back(1);
        
    hios.writeCode(pp);
    hios.~HuffmanIO(); //NOTE: destructor performs bitFlush() and flush()    
    
    ifstream input("./tests/testfiles/tmp2");
    for (int i = 0; i < 8; i++)
        EXPECT_TRUE((char) input.get() == (char) 255);
    EXPECT_TRUE((char) input.get() == '\0');    
    bool alright = true;
    for (int i = 0; i < 4096 + 5; i++)
    {
        if ((char) input.get() != (char)i)
            alright = false;
    }
    EXPECT_TRUE(alright);
    
    alright = true;
    for (int i = 0; i < 4096 + 5; i++)
    {
        if ((char) input.get() != (char) 65)
        {
            alright = false;
        }
    }
    
    EXPECT_TRUE(alright);
    
    EXPECT_TRUE((char) input.get() == (char)(255));
    EXPECT_TRUE((char) input.get() == (char)(255));
    EXPECT_TRUE((char) input.get() == (char)(3));
    
    input.get();
    EXPECT_TRUE(input.eof());
}

TEST(HuffmanTree_test, one_symbol_case)
{		
	HuffmanConstants::frequencies_t frequencies[255];
	memset(frequencies, 0, sizeof(HuffmanConstants::frequencies_t) * HuffmanConstants::number_of_byte_values);
	
	frequencies[0] = 1;
	HuffmanTree tree(frequencies);
	EXPECT_TRUE(tree.getCode(0).size() == 1);
	
}

TEST(HuffmanTree_test, many_symbols_case)
{
    HuffmanConstants::frequencies_t frequencies[255];
	memset(frequencies, 0, sizeof(HuffmanConstants::frequencies_t) * HuffmanConstants::number_of_byte_values);
	
	frequencies[127] = 1;
	frequencies[65] = 1;
	frequencies[198] = 1;
	frequencies[2] = 1;
	frequencies[0] = 1000000;
	
	HuffmanTree tree(frequencies);
	EXPECT_TRUE(tree.getCode(0).size() == 1);
	EXPECT_TRUE(tree.getCode(2).size() == 3);
	EXPECT_TRUE(tree.getCode(65).size() == 3);
	EXPECT_TRUE(tree.getCode(198).size() == 3);
	EXPECT_TRUE(tree.getCode(127).size() == 3);
	
	for (int i = 0; i < 256; i++)
	{
	    if (i != 0 && i != 2 && i != 65 && i != 198 && i != 127)
	        EXPECT_TRUE(tree.getCode(i).size() == 0);
	}
	
	EXPECT_TRUE(tree.getCode(2) != tree.getCode(65));
	EXPECT_TRUE(tree.getCode(2) != tree.getCode(198));
	EXPECT_TRUE(tree.getCode(2) != tree.getCode(127));
	EXPECT_TRUE(tree.getCode(65) != tree.getCode(198));
	EXPECT_TRUE(tree.getCode(65) != tree.getCode(127));
	EXPECT_TRUE(tree.getCode(198) != tree.getCode(127));
}

TEST(HuffmanWalker_test, one_symbol_case)
{
    HuffmanConstants::frequencies_t frequencies[255];
	memset(frequencies, 0, sizeof(HuffmanConstants::frequencies_t) * HuffmanConstants::number_of_byte_values);
	
	frequencies[2] = 1;
	HuffmanTree tree(frequencies);

	HuffmanTree::HuffmanTreeWalker walker(tree);
	walker.walk(0);
	EXPECT_TRUE(walker.isLeaf());
	EXPECT_TRUE(walker.getSymbol() == (uint8_t) 2);
	walker.walk(0);
	EXPECT_TRUE(walker.isLeaf());
	EXPECT_TRUE(walker.getSymbol() == (uint8_t) 2);
	walker.walk(0);
	EXPECT_TRUE(walker.isLeaf());
	EXPECT_TRUE(walker.getSymbol() == (uint8_t) 2);
}

TEST(HuffmanTreeWalker_test, many_symbols_case)
{
    HuffmanConstants::frequencies_t frequencies[255];
	memset(frequencies, 0, sizeof(HuffmanConstants::frequencies_t) * HuffmanConstants::number_of_byte_values);
	
	frequencies[127] = 1;
	frequencies[65] = 1;
	frequencies[198] = 1;
	frequencies[2] = 1;
	frequencies[0] = 1000000;
	
	HuffmanTree tree(frequencies);
	HuffmanTree::HuffmanTreeWalker walker(tree);
	walker.walk(1);
	EXPECT_TRUE(walker.isLeaf());
	EXPECT_TRUE(walker.getSymbol() == (uint8_t) 0);
	
	for (bool b : tree.getCode(65))
	    walker.walk(b);
	EXPECT_TRUE(walker.isLeaf());
	EXPECT_TRUE(walker.getSymbol() == (uint8_t) 65);
	
	for (bool b : tree.getCode(2))
	    walker.walk(b);
	EXPECT_TRUE(walker.isLeaf());
	EXPECT_TRUE(walker.getSymbol() == (uint8_t) 2);
}

TEST(HuffmanFileArchivet_test, empty_test_compress)
{
    string inp = "./tests/testfiles/empty_test";
    string outp = "./tests/testfiles/out";
    
    HuffmanConstants::input_parametrs prm(inp, outp, false);
    HuffmanFileArchiver arch(prm);
    
    arch.doTask(HuffmanConstants::COMPRESS);
    arch.closeFiles();
    
    HuffmanIO hios("./tests/testfiles/out", "./tests/testfiles/tmp");
    
    uint16_t number_of_not_empty_symbols;
    hios.readInt(number_of_not_empty_symbols);
    EXPECT_TRUE(number_of_not_empty_symbols == 0);
    
    uint64_t number_of_bits;
    hios.readInt(number_of_bits);
    EXPECT_TRUE(number_of_bits == 0);
    
    uint8_t h;
    hios.readInt(h);
    EXPECT_TRUE(hios.inputEof());
}

TEST(HuffmanFileArchivet_test, empty_test_decompress)
{
    string inp = "./tests/testfiles/empty_test";
    string outp = "./tests/testfiles/out";
    
    HuffmanConstants::input_parametrs prm(inp, outp, false);
    HuffmanFileArchiver arch(prm);
    arch.doTask(HuffmanConstants::COMPRESS);
    arch.closeFiles();
    
    inp = "./tests/testfiles/out";
    outp = "./tests/testfiles/tmp";
    HuffmanConstants::input_parametrs prm2(inp, outp, false);
    HuffmanFileArchiver arch2(prm2);
    arch2.doTask(HuffmanConstants::DECOMPRESS);
    arch2.closeFiles();
    
    ifstream res("./tests/testfiles/tmp");
    res.get();
    EXPECT_TRUE(res.eof());
    res.close();
}

TEST(HuffmanFileArchivet_test, one_byte_test_compress)
{
    string inp = "./tests/testfiles/one_byte_test";
    string outp = "./tests/testfiles/out";
    
    HuffmanConstants::input_parametrs prm(inp, outp, false);
    HuffmanFileArchiver arch(prm);
    
    arch.doTask(HuffmanConstants::COMPRESS);
    arch.closeFiles();
    
    HuffmanIO hios("./tests/testfiles/out", "./tests/testfiles/tmp");
    
    uint16_t number_of_not_empty_symbols;
    hios.readInt(number_of_not_empty_symbols);    
    EXPECT_TRUE(number_of_not_empty_symbols == 1);
    
    uint8_t h;
    hios.readInt(h);
    EXPECT_TRUE(h == 0);
    
    uint64_t freq;
    hios.readInt(freq);
    EXPECT_TRUE(freq == 1);
    
    uint64_t number_of_bits;
    hios.readInt(number_of_bits);
    EXPECT_TRUE(number_of_bits == 1);
    
    hios.readInt(h);
    EXPECT_TRUE(h == 0);
    
    hios.readInt(h);
    EXPECT_TRUE(hios.inputEof());
}

TEST(HuffmanFileArchivet_test, one_byte_test_decompress)
{
    string inp = "./tests/testfiles/one_byte_test";
    string outp = "./tests/testfiles/out";
    
    HuffmanConstants::input_parametrs prm(inp, outp, false);
    HuffmanFileArchiver arch(prm);
    
    arch.doTask(HuffmanConstants::COMPRESS);
    arch.closeFiles();
    
    string inp2 = "./tests/testfiles/out";
    string outp2 = "./tests/testfiles/tmp";
    HuffmanConstants::input_parametrs prm2(inp2, outp2, false);
    HuffmanFileArchiver arch2(prm2);
    arch2.doTask(HuffmanConstants::DECOMPRESS);
    arch2.closeFiles();
    
    ifstream input("./tests/testfiles/tmp");
    EXPECT_TRUE(input.get() == '\0');
    input.get();
    EXPECT_TRUE(input.eof());
    input.close();
}

TEST(HuffmanFileArchivet_test, many_bytes_compress_decompress)
{
    ofstream output("./tests/testfiles/many_bytes_test");
    output.put('\0');
    char s[256] = "privet kak dela всё хорошо спасибо тест тест тест \0";
    output.write(s, 256);
    output.put(1);
    output.close();
     
    string inp = "./tests/testfiles/many_bytes_test";
    string outp = "./tests/testfiles/out";
    
    HuffmanConstants::input_parametrs prm(inp, outp, false);
    HuffmanFileArchiver arch(prm);
    
    arch.doTask(HuffmanConstants::COMPRESS);
    arch.~HuffmanFileArchiver();
    
    string inp2 = "./tests/testfiles/out";
    string outp2 = "./tests/testfiles/tmp";
    HuffmanConstants::input_parametrs prm2(inp2, outp2, false);
    HuffmanFileArchiver arch2(prm2);
    arch2.doTask(HuffmanConstants::DECOMPRESS);
    arch2.~HuffmanFileArchiver();
    
    ifstream input("./tests/testfiles/tmp");
    EXPECT_TRUE(input.get() == '\0');
    char t[256];
    input.read(t, 256);
    
    bool alright = true;
    for (int i = 0; i < 256; i++)
    {
        if ((char)t[i] != (char)s[i])
        {
            alright = false;
        }
    }
    
    EXPECT_TRUE(alright);
    EXPECT_TRUE((char) input.get() == (char) 1);
    input.close();
}

TEST(HuffmanFileArchivet_test, many_bytes_compress)
{
    ofstream output("./tests/testfiles/many_bytes_test");
    for (int i = 0; i < 5; i++)
        output.put(1);
    output.put(0);
    for (int i = 0; i < 3; i++)
        output.put(2);
    output.put(0);
    for (int i = 0; i < 8; i++)
        output.put(3);
    output.put(0);
    for (int i = 0; i < 1; i++)
        output.put(4);
    output.put(0);
    
    output.close();
     
    string inp = "./tests/testfiles/many_bytes_test";
    string outp = "./tests/testfiles/out";
    
    HuffmanConstants::input_parametrs prm(inp, outp, false);
    HuffmanFileArchiver arch(prm);
    
    arch.doTask(HuffmanConstants::COMPRESS);
    
    arch.closeFiles();
    
    HuffmanIO hios("./tests/testfiles/out", "./tests/testfiles/tmp");
    
    uint16_t number_of_not_empty_symbols;
    hios.readInt(number_of_not_empty_symbols);    
    EXPECT_TRUE(number_of_not_empty_symbols == 5);
    
    uint8_t h;
    hios.readInt(h);
    EXPECT_TRUE(h == 0);

    uint64_t freq;
    hios.readInt(freq);
    EXPECT_TRUE(freq == 4);
    
    hios.readInt(h);
    EXPECT_TRUE(h == 1);
    hios.readInt(freq);
    EXPECT_TRUE(freq == 5);
    
    hios.readInt(h);
    EXPECT_TRUE(h == 2);
    hios.readInt(freq);
    EXPECT_TRUE(freq == 3);
    
    hios.readInt(h);
    EXPECT_TRUE(h == 3);
    hios.readInt(freq);
    EXPECT_TRUE(freq == 8);
    
    hios.readInt(h);
    EXPECT_TRUE(h == 4);
    hios.readInt(freq);
    EXPECT_TRUE(freq == 1);
}
