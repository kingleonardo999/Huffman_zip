#ifndef HUFFMAN_TREE_STORAGE_H
#define HUFFMAN_TREE_STORAGE_H

#include <fstream>
#include <string>
#include <map>
#include "huffman_tree.hpp"
#include <iostream>

using namespace std;

void saveHuffmanTreeToFile(TreeNode* root, string filename1, string filename2);

TreeNode* GenHuffTreeByConfigFile(const string& Struct_content, const string& ASCII_content);

#endif // HUFFMAN_TREE_STORAGE_H