#ifndef HUFFMAN_TREE
#define HUFFMAN_TREE

#include <iostream>
#include <utility>
#include <stdint.h>
#include <vector>
#include <queue>
#include "HuffmanConstants.h"

class HuffmanTree
{
public:
    ~HuffmanTree();
    HuffmanTree(const HuffmanConstants::frequencies_t frequencies[]);
    
    HuffmanConstants::packedBits_t& getCode(uint8_t h);
    class HuffmanTreeWalker;
private:
    class TreeNode;
    TreeNode* root;
    HuffmanConstants::packedBits_t new_code[HuffmanConstants::number_of_byte_values];
    
    void destroyingDfs(TreeNode* v);
    void setsNewCodesWithDfs(TreeNode* v, HuffmanConstants::packedBits_t& code);
    
    HuffmanTree(HuffmanTree& other);
    HuffmanTree& operator=(HuffmanTree& other);
};

class HuffmanTree::TreeNode
{
    friend class HuffmanTree;
    public:
        TreeNode(){};
        TreeNode(TreeNode*, TreeNode*, uint8_t);
        bool isLeaf() const;
    private:
        TreeNode* left_son;
        TreeNode* right_son;
        uint8_t leaf_symbol; //if node is leaf
        
        TreeNode(TreeNode& other);
        TreeNode& operator=(TreeNode& other);
};

class HuffmanTree::HuffmanTreeWalker
{
    public:
        HuffmanTreeWalker(HuffmanTree& tree);
        
        void walk(bool bit);
        bool isLeaf() const;
        uint8_t getSymbol() const;
    private:
        HuffmanTree::TreeNode* current_node;
        HuffmanTree::TreeNode* root;
};
#endif
