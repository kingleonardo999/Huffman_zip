#include "huffman_tree.hpp"

// �Զ���ȽϺ��������ڹ�����С�� 
struct Compare {
    bool operator()(const TreeNode* l, const TreeNode* r) {
        return l->freq > r->freq;
    }
};

TreeNode* buildHuffmanTree(const map<char, int>& freqMap) {
    priority_queue<TreeNode*, vector<TreeNode*>, Compare> pq;

    // �����нڵ�������ȶ��� 
    for (const auto& pair : freqMap) {
        pq.push(new TreeNode(pair.first, pair.second));
    }

    // ������������ 
    while (pq.size() > 1) {
        TreeNode* left = pq.top(); pq.pop();
        TreeNode* right = pq.top(); pq.pop();

        TreeNode* sum = new TreeNode('\0', left->freq + right->freq);
        sum->left = left;
        sum->right = right;

        pq.push(sum);
    }

    return pq.empty() ? nullptr : pq.top();
}

// ����ʵ�ֹ��������� 
void generateCode(TreeNode* root, map<char, string>& huffmanCodes) {
    if (root == nullptr) return;

    stack<pair<TreeNode*, string>> s;
    s.push(make_pair(root, ""));

    while (!s.empty()) {
        auto current = s.top();
        s.pop();

        TreeNode* node = current.first;
        string code = current.second;

        if (node->character != '\0') {
            huffmanCodes[node->character] = code;
        }

        if (node->left) {
            s.push(make_pair(node->left, code + "0"));
        }
        if (node->right) {
            s.push(make_pair(node->right, code + "1"));
        }
    }
}
