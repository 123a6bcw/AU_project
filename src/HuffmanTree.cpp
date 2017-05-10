#include "HuffmanTree.h"

HuffmanTree::HuffmanTree(const HuffmanConstants::frequencies_t frequencies[])
{
    typedef std::pair <HuffmanConstants::frequencies_t, TreeNode*> NodeType; //first is frequence of the node, second is pointer to the node
    std::priority_queue<NodeType, std::vector<NodeType>, std::greater<NodeType> > nodes; //greater, so it will be min_heap
    uint8_t i = 0; //enumeration of all byte values
    do
    {
        if (frequencies[i] > 0)
        {
            TreeNode* new_node = new TreeNode(NULL, NULL, i);
            nodes.push({frequencies[i], new_node});
        }
        
        i++;
    } while (i != 0);
    
    if (nodes.size() == 0)
    {
        //case of empty input
        root = new TreeNode(NULL, NULL, 0);
        return;
    }
    
    while (nodes.size() > 1)
    {
        NodeType first_node = nodes.top();
        nodes.pop();
        NodeType second_node = nodes.top();
        nodes.pop();
        //got two nodes with the smallest frequency
        
        TreeNode* new_node = new TreeNode(first_node.second, second_node.second, 0); //combine two nodes by their parent, symbol is useless
        HuffmanConstants::frequencies_t new_frequency = first_node.first + second_node.first; //frequency of parent equals to sum of childs freq.
        nodes.push({new_frequency, new_node});
    }
    
    root = nodes.top().second;
    
    HuffmanConstants::packedBits_t code;
    setsNewCodesWithDfs(root, code);
    
    if (root->isLeaf())
    //That means there is only one symbol in input file. This function sets it's code to "0"
    {
        code.push_back(0);
        setsNewCodesWithDfs(root, code);
    }
}

void HuffmanTree::setsNewCodesWithDfs(TreeNode* v, HuffmanConstants::packedBits_t& code)
{
    if (v == NULL)
    {
        return;
    }
    
    if (v->isLeaf())
    {
        new_code[v->leaf_symbol] = code;
        return;
    }
    
    code.push_back(0);
    setsNewCodesWithDfs(v->left_son, code);
    
    code[code.size() - 1] = 1;
    setsNewCodesWithDfs(v->right_son, code);
    
    code.erase(code.begin() + code.size() - 1);
}

void HuffmanTree::destroyingDfs(TreeNode* v)
{
    if (v->left_son != NULL)
    {
        destroyingDfs(v->left_son);
    }
    
    if (v->right_son != NULL)
    {
        destroyingDfs(v->right_son);
    }
    
    delete v;
}

HuffmanTree::~HuffmanTree()
{
    destroyingDfs(root);
}

HuffmanConstants::packedBits_t& HuffmanTree::getCode(uint8_t h)
{
    return new_code[h];
}

HuffmanTree::TreeNode::TreeNode(HuffmanTree::TreeNode* l, HuffmanTree::TreeNode* r, uint8_t h): left_son(l), right_son(r), leaf_symbol(h){}

bool HuffmanTree::TreeNode::isLeaf() const
{
    return (left_son == NULL && right_son == NULL);
}

HuffmanTree::HuffmanTreeWalker::HuffmanTreeWalker(HuffmanTree& tree):current_node(tree.root), root(tree.root){}

void HuffmanTree::HuffmanTreeWalker::walk(bool bit)
{
    if (current_node->isLeaf())
    {
        current_node = root;
    }
    
    current_node = bit ? current_node->right_son : current_node->left_son;
    
    if (current_node == NULL)
    //happens only if root is leaf
    {
        current_node = root;
    }
}

bool HuffmanTree::HuffmanTreeWalker::isLeaf() const
{
    return current_node->isLeaf();
}

uint8_t HuffmanTree::HuffmanTreeWalker::getSymbol() const
{
    return current_node->leaf_symbol;
}
