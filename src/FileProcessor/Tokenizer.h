#ifndef READER
#define READER

#include "Codes.h"
#include "FileData.h"
#include <memory>
#include <sstream>
#include <string>

using namespace std;

class Tokenizer{
public:
    CFFile* FileToRead;
    bool TrimSpecialChar;

    bool TokenizeFile(CFFile* f, bool trimspecialchar);

    void GetCodes(char c);
private:
    int CurrentLine = 0;
    int CurrentReadingPos = 0;
    int CodeStartPos = 0;
    int CodeEndPos = 0;
    int CodeLine = 0;
    string CurrentCode;
    BasicCodeTypes CodeType;
    bool isEscapeNext = false;

    int PushSymbol();

    // State processing methods
    bool ProcessCurrentState(char c);
    bool ProcessSingleLineComment(char c);
    bool ProcessMultiLineComment(char c);
    bool ProcessTextState(char c);
    bool ProcessNumberState(char c);
    bool ProcessNormalCodeState(char c);
    bool ProcessCharState(char c);

    // Character processing methods
    void ProcessNewCharacter(char c);
    void ProcessNewline(char c);
    void ProcessWhitespace();
    void ProcessNumberStart(char c);
    void ProcessAlphabetStart(char c);
    void ProcessSpecialChar(char c);
};



#endif
