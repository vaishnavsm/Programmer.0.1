#include "utils.h"

#include "string"
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

int strToInt(const string &text)
{
    stringstream ss(text);
    int result;
    return ss >> result ? result : 0;
}

string intToStr(int number)
{
    stringstream ss;
    ss << number;
    return ss.str();
}

vector<string> split(const string &s, const string &delim, const bool keep_empty)
{
    vector<string> ret;

    if(delim.empty())
        {
            ret.push_back(s);
            return ret;
        }

    string::const_iterator sStart = s.begin(), sEnd;

    while(true)
        {
            sEnd = search(sStart, s.end(), delim.begin(), delim.end());
            string ins(sStart, sEnd);

            if(keep_empty || !ins.empty())
                {
                    ret.push_back(ins);
                }

            if(sEnd == s.end())
                {
                    break;
                }

            sStart = sEnd + delim.size();
        }

    return ret;
}


