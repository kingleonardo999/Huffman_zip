#include "huffman_tree_storage.hpp"

// 迭代实现先序遍历
bool preOrderTraversal(TreeNode* root, string& hstruct, string& hascii) {
    stack<TreeNode*> s;
    s.push(root);

    while (!s.empty()) {
        TreeNode* node = s.top();
        s.pop();

        if (node != nullptr) {
            if(node->isLeaf()){
                hstruct += '0';
                hascii += node->character;
            } else{
                hstruct += '1';
            }

            if (node->right) {
                s.push(node->right);
            }
            if (node->left) {
                s.push(node->left);
            }
        }
    }
    return true;
}

void saveHuffmanTreeToFile(TreeNode* root, string filename1, string filename2){
    string H_Struct,H_ASCII;
    preOrderTraversal(root, H_Struct, H_ASCII);
    writeFile(filename1, H_Struct);
    writeFile(filename2, H_ASCII);
}

TreeNode* GenHuffTreeByConfigFile(const string& Struct_content, const string& ASCII_content){
    stack<TreeNode*> nodeStack;
    TreeNode *root = new TreeNode();
    nodeStack.push(root);
    int asciiIndex = 0; // 用于跟踪ASCII_content中的索引

    for(int i = 0; i < Struct_content.size(); i++){
        TreeNode* node = nodeStack.top();
        nodeStack.pop();
        if(Struct_content[i] == '1'){
            node->left = new TreeNode();
            node->right = new TreeNode();
            nodeStack.push(node->right);
            nodeStack.push(node->left);
        }
        else if(Struct_content[i] == '0'){
            node->character = ASCII_content[asciiIndex++];
        }
        else {
            return nullptr;
        }
    }
    return root;
}
