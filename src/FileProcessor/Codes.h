#ifndef CODES
#define CODES

#include <string>

using namespace std;

enum BasicCodeTypes: int{
    SINGLELINECOMMENT, MULTILINECOMMENTSTART, MULTILINECOMMENTEND, TYPE, NUM, BYTE, STRING, KEYWORD
};

string& GetMultilineCommentEndSym();

bool IsAcceptableCodeNameChar(char c);
bool IsAcceptatbleNumByteChar(char c);

void AddCFCode(const string &Keyword, unsigned short Code);
void AddCustKeyword(const string &Code , const string &Keyword);
int GetCodeFromKeyword(const string &Keyword);
bool AcceptSucceedingSpecialChar(const string &Text, char C);

#endif
