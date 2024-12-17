#include "frequency_counter.hpp"

map<char, int> countFrequencies(const string& text) {
    map<char, int> frequencyMap;
    for (char c : text) {
        ++frequencyMap[c];
    }
    return frequencyMap;
}