#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector> 

using namespace std;

bool readFile(const string& filename, string& content);
bool readFile(const string& filename, vector<uint8_t>& data);
bool writeFile(const string& filename, const string& content);
bool writeFile(const string& filename, const vector<uint8_t>& data);

string getUserInput();

#endif // FILE_IO_H
