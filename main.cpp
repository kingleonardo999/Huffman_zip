#include <iostream>
#include <string>
#include <fstream>
#include "compression_decompression.hpp"
#include "file_io.hpp"
#include "huffman_tree.hpp"
#include "huffman_tree_storage.hpp"
#include "frequency_counter.hpp"

using namespace std;
// 显示菜单并获取用户的选择 
int showMenuAndGetChoice() {
    int choice;
    while (true) {
        cout << "1. 压缩文件\n";
        cout << "2. 解压文件\n";
        cout << "3. 退出\n";
        cout << "请输入操作(1 or 2 or 3): ";

        if (cin >> choice) {
            if (choice == 1 || choice == 2 || choice == 3) {
                return choice;
            }
            cout << "非法输入，请再次尝试\n";
        } else {
            cin.clear(); // 清除错误标志 
            cout << "非法输入，请输入一个操作.\n";
        }
    }
}

// 主函数
int main() {
    while (true) {
        int choice = showMenuAndGetChoice();

        string filePath;

        if (choice == 1) {
            cout << "请输入待压缩文件地址: ";
            filePath = getUserInput();
            if (compress(filePath)) {
                cout << "文件压缩成功!\n";
            } else {
                cout << "文件压缩失败!\n";
            }
        } else if (choice == 2) {
            cout << "请输入待解压文件地址: ";
            filePath = getUserInput();
            if (uncompress(filePath)) {
                cout << "文件解压成功!\n";
            } else {
                cout << "文件解压失败!\n";
            }
        } else if (choice == 3) break;

        char continueChoice;
        cout << "是否继续压缩或解压文件?(y/n): ";
        cin >> continueChoice;
        cout << endl;

        if (continueChoice != 'y' && continueChoice != 'Y') {
            break;
        }
        system("cls"); 
    }

    cout << "程序结束.\n";
    return 0;
}
