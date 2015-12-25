#ifndef UTILS_H
#define UTILS_H

#include "string"
#include <vector>

using namespace std;

int strToInt(const string &text);
string intToStr(int number);
vector<string> split(const string &s, const string &delim, const bool keep_empty);

#endif // UTILS_H
