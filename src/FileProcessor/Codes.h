#ifndef CODES
#define CODES

#include <string>

using namespace std;

enum BasicCodeTypes: int{
    NONE, SINGLELINECOMMENT, MULTILINECOMMENTSTART, MULTILINECOMMENTEND, MULTILINECOMMENT,
    TYPE, NUM, BYTE, STRING, KEYWORD, CHAR, NORMALCODE, FUNCTION,
    TAGS, TAGSYMBOL
};

string& GetMultilineCommentEndSym();

bool IsAcceptableCodeNameChar(char c);
bool IsAcceptatbleNumByteChar(char c);

void AddCFCode(const string &Keyword, unsigned short Code);
void AddCustKeyword(const string &Code , const string &Keyword);
int GetCodeFromKeyword(const string &Keyword);
bool AcceptSucceedingSpecialChar(const string &Text, char C);

#endif
