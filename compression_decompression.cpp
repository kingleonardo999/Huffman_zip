#include "compression_decompression.hpp"
#include <cstdint>

// 用于去除最后一个字符c打头的后缀
string removeExtension(const string& filename, char c) {
    size_t lastc = filename.find_last_of(c);

    // 如果没有找到c，则直接返回原文件名
    if (lastc == string::npos) {
        return filename;
    }

    // 否则，去除后缀
    return filename.substr(0, lastc);
}

// 获取父目录
string removeExtension(const string& filename) {
    size_t lastc = filename.find_last_of('/');

    // 如果没有找到，则直接返回原文件名
    if (lastc == string::npos) {
        lastc = filename.find_last_of('\\');
        if(lastc == string::npos)
            return filename;
    }

    // 否则，去除后缀
    return filename.substr(0, lastc);
}

// 将二进制字符串转换为字节数组
vector<uint8_t> binaryStringToBytes(const string& binaryString) {
    vector<uint8_t> bytes;
    size_t length = binaryString.length();
    uint8_t byte = 0;
    int bitCount = 0;

    for (size_t i = 0; i < length; ++i) {
        // 从右边开始填充byte变量
        if (binaryString[i] == '1') {
            byte |= (1 << bitCount);
        }
        ++bitCount;
        if (bitCount == 8) {
            bytes.push_back(byte);
            byte = 0;
            bitCount = 0;
        }
    }

    // 处理最后一组不足8位的情况
    if (bitCount > 0) {
        bytes.push_back(byte); // 最后一个字节可能不足8位
    }

    return bytes;
}

bool compress(const string& filePath) {
    // 1.打开文件读取内容
    string source_content;
    if(!readFile(filePath, source_content)) {
        cout << "读取待压缩文件失败：" << filePath << endl;
        return false;
    }

    // 2.记录频率
    map<char, int> m = countFrequencies(source_content);

    // 3.建立哈夫曼树
    TreeNode* root = buildHuffmanTree(m);
    if(!root) {
        cout << "创建哈夫曼树失败！" << endl;
        return false;
    }

    // 4.去除文件后缀并创建 config1.txt 和config2.txt 分别存储哈夫曼树的结构和叶节点信息
    string filenameWithoutExtension = removeExtension(filePath, '.');
    string newFileName1 = filenameWithoutExtension + "config1.txt";
    string newFileName2 = filenameWithoutExtension + "config2.txt";

    saveHuffmanTreeToFile(root, newFileName1, newFileName2);

    // 5.转换源文件得到压缩文件
    map<char, string> huffmanCodes;
    generateCode(root, huffmanCodes);
    string newFileName3 = filePath + ".zip";
    string destination_content;
    for(char i : source_content){
        destination_content += huffmanCodes[i];
    }

    // 将二进制字符串转换为字节数组
    vector<uint8_t> bytes = binaryStringToBytes(destination_content);
    // 计算最后一个字节的有效长度
    int lastByteLength = (destination_content.length() % 8) ? (destination_content.length() % 8) : 8;

    // 在文件开头添加标志位
    vector<uint8_t> fileData;
    fileData.push_back(static_cast<uint8_t>(lastByteLength)); // 标志位
    fileData.insert(fileData.end(), bytes.begin(), bytes.end());

    if(!writeFile(newFileName3, fileData)) {
        cout << "写入文件失败：" << newFileName3 << endl;
        return false;
    }

    delete root;
    return true;
}

// 将二进制的标志位转化为十进制
int binaryStringToDecimal(const string& binaryStr) {
    if (binaryStr.length() != 8) {
        throw std::invalid_argument("二进制字符串长度必须为8！");
    }
    int decimal = 0;
    for (int i = 0; i < 8; ++i) {
        if (binaryStr[i] == '1') {
            decimal += (1 << i);
        } else if (binaryStr[i] != '0') {
            throw std::invalid_argument("二进制字符串包含非法输入！");
        }
    }
    return decimal;
}

// 将字节数组转换为二进制字符串
string bytesToBinaryString(const vector<uint8_t>& bytes) {
    string binaryStr;
    for (auto byte : bytes) {
        for (int i = 0; i < 8; ++i) {
            binaryStr += (byte >> i) & 1 ? '1' : '0';
        }
    }
    return binaryStr;
}

bool uncompress(const string& filePath){
    // 1.搜索当前目录下有无哈夫曼树的结构和信息文件，没有则让用户手动输入两个文件地址
    string filenameWithoutExtension = removeExtension(filePath,'.');
    filenameWithoutExtension = removeExtension(filenameWithoutExtension,'.');
    string newFileName1 = filenameWithoutExtension + "config1.txt";
    string newFileName2 = filenameWithoutExtension + "config2.txt";
    string Struct_content,ASCII_content;
    if(!readFile(newFileName1,Struct_content) || !readFile(newFileName2, ASCII_content)) {
        cout << "请输入存放哈夫曼树结构的文件地址：";
        newFileName1 = getUserInput();
        cout << "请输入存放哈夫曼树叶节点信息的文件地址：";
        newFileName2 = getUserInput();
        if(!readFile(newFileName1,Struct_content) || !readFile(newFileName2, ASCII_content)) {
            cout << "哈夫曼树结构文件或叶节点信息文件有误！" << endl;
            return false;
        }
    }

    // 2.创建哈夫曼树
    TreeNode *root = GenHuffTreeByConfigFile(Struct_content,ASCII_content);
    if(!root) {
        cout << "创建哈夫曼树失败！" << endl;
        return false;
    }

    // 3.读取源文件内容
    string source_content;
    vector<uint8_t> bytes;
    if(!readFile(filePath,bytes)) {
        cout << "待解压的文件不存在！";
        return false;
    }
    source_content = bytesToBinaryString(bytes);
    //处理标志位和最后一位
    int lastByteLength = binaryStringToDecimal(source_content.substr(0,8));
    // 处理最后一个字节的有效长度
    source_content = source_content.substr(8, source_content.size() - 8 -(8 - lastByteLength));
	
    // 4.存放解压文件内容
    string destination_content;
    TreeNode *p = root;
    for(char i : source_content){
        switch (i)
        {
        case '0':
            p = p->left;
            break;
        case '1':
            p = p->right;
            break;
        default:
            cout << "待解压文件的内容有误！";
            return false;
        }
        if(p->isLeaf()){
            destination_content += p->character;
            p = root;
        }
    }

    // 5.写入目标文件
    string destination = filenameWithoutExtension + "_unzip.txt";
    if(!writeFile(destination, destination_content)) {
        cout << "写入文件失败！ " << destination << endl;
        return false;
    }

    delete root;
    return true;
}
