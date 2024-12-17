#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <stack>

#include"file_io.hpp"
#include"frequency_counter.hpp"

using namespace std;

class TreeNode {
public:
    char character;  // ×Ö·û 
    int freq;       // ÆµÂÊ 
    TreeNode* left;
    TreeNode* right;

    TreeNode(char ch, int freq) : character(ch), freq(freq), left(nullptr), right(nullptr) {}
    TreeNode(char ch) : character(ch), freq(0), left(nullptr), right(nullptr) {}
    TreeNode() = default;

    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }

    ~TreeNode() {
        delete left;
        delete right;
    }

};

TreeNode* buildHuffmanTree(const map<char, int>& freqMap);

void generateCode(TreeNode* root, map<char, string>& huffmanCodes);

#endif // HUFFMAN_TREE_H
