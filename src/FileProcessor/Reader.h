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

    void GetCodes(const char &c);
private:
    int CurrentLine = 0;
    int CurrentReadingPos = 0;
    int CodeStartPos = 0;
    int CodeEndPos = 0;
    int CodeLine = 0;
    string CurrentCode;
    CFCodeType CodeType;

    int PushSymbol();
};

bool IsAcceptableCodeNameChar(char c);
bool IsAcceptatbleNumByteChar(char c);

#endif
