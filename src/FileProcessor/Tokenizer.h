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
    LangTemp *Template;

    bool TokenizeFile(CFFile* f, LangTemp* temp);// pass null as Template means it will be read as template

    void TokenizeCodes(char c);
    void TokenizeTemplate(char c);
private:
    int CurrentLine = 1;
    int CurrentLineCharPos = 1;// the position in the line
    int CurrentReadingPos = 0;// the position in the whole file

    int CodeStartPos = 0;
    int CodeEndPos = 0;

    int CodeStartLine = 0, CodeStartLinePos = 0;
    int CodeEndLine = 0, CodeEndLinePos = 0;
    string CurrentCode;
    BasicCodeTypes CodeType;
    bool isEscapeNext = false;

    void MarkTokenStart();
    void MarkTokenEnd(int difference);
    // difference move the ending pos forward or backward from the current pos
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
