#include "compression_decompression.hpp"
#include <cstdint>

// ����ȥ�����һ���ַ�c��ͷ�ĺ�׺
string removeExtension(const string& filename, char c) {
    size_t lastc = filename.find_last_of(c);

    // ���û���ҵ�c����ֱ�ӷ���ԭ�ļ���
    if (lastc == string::npos) {
        return filename;
    }

    // ����ȥ����׺
    return filename.substr(0, lastc);
}

// ��ȡ��Ŀ¼
string removeExtension(const string& filename) {
    size_t lastc = filename.find_last_of('/');

    // ���û���ҵ�����ֱ�ӷ���ԭ�ļ���
    if (lastc == string::npos) {
        lastc = filename.find_last_of('\\');
        if(lastc == string::npos)
            return filename;
    }

    // ����ȥ����׺
    return filename.substr(0, lastc);
}

// ���������ַ���ת��Ϊ�ֽ�����
vector<uint8_t> binaryStringToBytes(const string& binaryString) {
    vector<uint8_t> bytes;
    size_t length = binaryString.length();
    uint8_t byte = 0;
    int bitCount = 0;

    for (size_t i = 0; i < length; ++i) {
        // ���ұ߿�ʼ���byte����
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

    // �������һ�鲻��8λ�����
    if (bitCount > 0) {
        bytes.push_back(byte); // ���һ���ֽڿ��ܲ���8λ
    }

    return bytes;
}

bool compress(const string& filePath) {
    // 1.���ļ���ȡ����
    string source_content;
    if(!readFile(filePath, source_content)) {
        cout << "��ȡ��ѹ���ļ�ʧ�ܣ�" << filePath << endl;
        return false;
    }

    // 2.��¼Ƶ��
    map<char, int> m = countFrequencies(source_content);

    // 3.������������
    TreeNode* root = buildHuffmanTree(m);
    if(!root) {
        cout << "������������ʧ�ܣ�" << endl;
        return false;
    }

    // 4.ȥ���ļ���׺������ config1.txt ��config2.txt �ֱ�洢���������Ľṹ��Ҷ�ڵ���Ϣ
    string filenameWithoutExtension = removeExtension(filePath, '.');
    string newFileName1 = filenameWithoutExtension + "config1.txt";
    string newFileName2 = filenameWithoutExtension + "config2.txt";

    saveHuffmanTreeToFile(root, newFileName1, newFileName2);

    // 5.ת��Դ�ļ��õ�ѹ���ļ�
    map<char, string> huffmanCodes;
    generateCode(root, huffmanCodes);
    string newFileName3 = filePath + ".zip";
    string destination_content;
    for(char i : source_content){
        destination_content += huffmanCodes[i];
    }

    // ���������ַ���ת��Ϊ�ֽ�����
    vector<uint8_t> bytes = binaryStringToBytes(destination_content);
    // �������һ���ֽڵ���Ч����
    int lastByteLength = (destination_content.length() % 8) ? (destination_content.length() % 8) : 8;

    // ���ļ���ͷ��ӱ�־λ
    vector<uint8_t> fileData;
    fileData.push_back(static_cast<uint8_t>(lastByteLength)); // ��־λ
    fileData.insert(fileData.end(), bytes.begin(), bytes.end());

    if(!writeFile(newFileName3, fileData)) {
        cout << "д���ļ�ʧ�ܣ�" << newFileName3 << endl;
        return false;
    }

    delete root;
    return true;
}

// �������Ƶı�־λת��Ϊʮ����
int binaryStringToDecimal(const string& binaryStr) {
    if (binaryStr.length() != 8) {
        throw std::invalid_argument("�������ַ������ȱ���Ϊ8��");
    }
    int decimal = 0;
    for (int i = 0; i < 8; ++i) {
        if (binaryStr[i] == '1') {
            decimal += (1 << i);
        } else if (binaryStr[i] != '0') {
            throw std::invalid_argument("�������ַ��������Ƿ����룡");
        }
    }
    return decimal;
}

// ���ֽ�����ת��Ϊ�������ַ���
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
    // 1.������ǰĿ¼�����޹��������Ľṹ����Ϣ�ļ���û�������û��ֶ����������ļ���ַ
    string filenameWithoutExtension = removeExtension(filePath,'.');
    filenameWithoutExtension = removeExtension(filenameWithoutExtension,'.');
    string newFileName1 = filenameWithoutExtension + "config1.txt";
    string newFileName2 = filenameWithoutExtension + "config2.txt";
    string Struct_content,ASCII_content;
    if(!readFile(newFileName1,Struct_content) || !readFile(newFileName2, ASCII_content)) {
        cout << "�������Ź��������ṹ���ļ���ַ��";
        newFileName1 = getUserInput();
        cout << "�������Ź�������Ҷ�ڵ���Ϣ���ļ���ַ��";
        newFileName2 = getUserInput();
        if(!readFile(newFileName1,Struct_content) || !readFile(newFileName2, ASCII_content)) {
            cout << "���������ṹ�ļ���Ҷ�ڵ���Ϣ�ļ�����" << endl;
            return false;
        }
    }

    // 2.������������
    TreeNode *root = GenHuffTreeByConfigFile(Struct_content,ASCII_content);
    if(!root) {
        cout << "������������ʧ�ܣ�" << endl;
        return false;
    }

    // 3.��ȡԴ�ļ�����
    string source_content;
    vector<uint8_t> bytes;
    if(!readFile(filePath,bytes)) {
        cout << "����ѹ���ļ������ڣ�";
        return false;
    }
    source_content = bytesToBinaryString(bytes);
    //�����־λ�����һλ
    int lastByteLength = binaryStringToDecimal(source_content.substr(0,8));
    // �������һ���ֽڵ���Ч����
    source_content = source_content.substr(8, source_content.size() - 8 -(8 - lastByteLength));
	
    // 4.��Ž�ѹ�ļ�����
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
            cout << "����ѹ�ļ�����������";
            return false;
        }
        if(p->isLeaf()){
            destination_content += p->character;
            p = root;
        }
    }

    // 5.д��Ŀ���ļ�
    string destination = filenameWithoutExtension + "_unzip.txt";
    if(!writeFile(destination, destination_content)) {
        cout << "д���ļ�ʧ�ܣ� " << destination << endl;
        return false;
    }

    delete root;
    return true;
}
