#You can change this values
CC      := g++
EXT     := cpp
FG		:= -std=c++11 -g -Wall -Wshadow -Wextra -pedantic
PROGNAME:= huffman

INCDIR  := include
OBJDIR  := bin
SRCDIR  := src
DEPDIR  := dep

TESTDIR := tests
TESTFILE:= test


#Don't change this, if you don't know what is it
SOURCES := $(shell find $(SRCDIR) -name '*.$(EXT)')
OBJECTS := $(subst $(SRCDIR), $(OBJDIR), $(SOURCES:.$(EXT)=.o))
DEPS    := $(subst $(SRCDIR), $(DEPDIR), $(SOURCES:.$(EXT)=.d))

TESTS	:= $(shell find $(TESTDIR) -name '*.$(EXT)')
OBJTEST := $(subst $(TESTDIR), $(OBJDIR), $(TESTS:.$(EXT)=.o))

.PHONY: dirs all clean val run std

all: dirs $(PROGNAME)

dirs:
	mkdir -p $(OBJDIR) $(DEPDIR)



$(TESTFILE): dirs $(OBJTEST) $(OBJDIR)/HuffmanFileArchiver.o $(OBJDIR)/HuffmanIO.o $(OBJDIR)/HuffmanTree.o
	$(CC) $(FG) $(OBJTEST) $(OBJDIR)/HuffmanFileArchiver.o $(OBJDIR)/HuffmanIO.o $(OBJDIR)/HuffmanTree.o -lgtest_main -lgtest -lpthread -o $(TESTFILE)

$(OBJDIR)/test.o: $(TESTDIR)/test.cpp include/HuffmanFileArchiver.h
	$(CC) $(FG) -I$(INCDIR) -c $(TESTDIR)/test.cpp -o $(OBJDIR)/test.o

$(OBJDIR)/huffman_test.o: $(TESTDIR)/huffman_test.cpp include/HuffmanFileArchiver.h
	$(CC) $(FG) -I$(INCDIR) -c $(TESTDIR)/huffman_test.cpp -o $(OBJDIR)/huffman_test.o



$(PROGNAME): $(OBJECTS)
	$(CC) $(FG) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.$(EXT) 
	$(CC) $(FG) -MMD -I$(INCDIR) $< -c -o $@ 
	mv $(OBJDIR)/*.d $(DEPDIR)/
-include $(DEPS)



std:
	clear; clear; clear; make -B;

run:
	./$(PROGNAME)

val:
	valgrind ./$(PROGNAME)

clean:
	rm -rf $(PROGNAME) $(OBJDIR) $(DEPDIR) $(TESTFILE) *.test

#Makefile by Kostya and Misha ;)
