#ifndef COMPRESSION_DECOMPRESSION_H
#define COMPRESSION_DECOMPRESSION_H

#include <string>
#include <bitset>
#include <fstream>
#include <map>
#include "file_io.hpp"
#include "huffman_tree.hpp"
#include "huffman_tree_storage.hpp"
#include "frequency_counter.hpp"

using namespace std;

bool compress(const string& filePath);

bool uncompress(const string& filePath);

#endif // COMPRESSION_DECOMPRESSION_H
