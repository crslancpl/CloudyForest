#ifndef READER
#define READER

#include "FileData.h"
#include <memory>
#include <sstream>
#include <string>

using namespace std;

class Reader{
public:
    CFFile* FileToRead;
    bool TrimSpecialChar;
    bool ReadFile(CFFile* f, bool trimspecialchar);
    int PushSymbol(string &Symbol);
    void GetCodes(const char &c);
};



#endif
