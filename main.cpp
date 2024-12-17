#include <iostream>
#include <string>
#include <fstream>
#include "compression_decompression.hpp"
#include "file_io.hpp"
#include "huffman_tree.hpp"
#include "huffman_tree_storage.hpp"
#include "frequency_counter.hpp"

using namespace std;
// ��ʾ�˵�����ȡ�û���ѡ�� 
int showMenuAndGetChoice() {
    int choice;
    while (true) {
        cout << "1. ѹ���ļ�\n";
        cout << "2. ��ѹ�ļ�\n";
        cout << "3. �˳�\n";
        cout << "���������(1 or 2 or 3): ";

        if (cin >> choice) {
            if (choice == 1 || choice == 2 || choice == 3) {
                return choice;
            }
            cout << "�Ƿ����룬���ٴγ���\n";
        } else {
            cin.clear(); // ��������־ 
            cout << "�Ƿ����룬������һ������.\n";
        }
    }
}

// ������
int main() {
    while (true) {
        int choice = showMenuAndGetChoice();

        string filePath;

        if (choice == 1) {
            cout << "�������ѹ���ļ���ַ: ";
            filePath = getUserInput();
            if (compress(filePath)) {
                cout << "�ļ�ѹ���ɹ�!\n";
            } else {
                cout << "�ļ�ѹ��ʧ��!\n";
            }
        } else if (choice == 2) {
            cout << "���������ѹ�ļ���ַ: ";
            filePath = getUserInput();
            if (uncompress(filePath)) {
                cout << "�ļ���ѹ�ɹ�!\n";
            } else {
                cout << "�ļ���ѹʧ��!\n";
            }
        } else if (choice == 3) break;

        char continueChoice;
        cout << "�Ƿ����ѹ�����ѹ�ļ�?(y/n): ";
        cin >> continueChoice;
        cout << endl;

        if (continueChoice != 'y' && continueChoice != 'Y') {
            break;
        }
        system("cls"); 
    }

    cout << "�������.\n";
    return 0;
}
