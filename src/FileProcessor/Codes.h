#ifndef CODES
#define CODES

#include <string>

using namespace std;

enum BasicCodeTypes: int{
    SINGLELINECOMMENT, MULTILINECOMMENTSTART, MULTILINECOMMENTEND, TYPE, NUM, BYTE, STRING, KEYWORD
};

bool IsNumberChar(char c);
bool IsAlphabetChar(char c);

bool StartWith(const string &Text, const string &Pattern);
bool EndWith(const string &Text, const string &Pattern);

void AddCFCode(const string &Keyword, unsigned short Code);
void AddCustKeyword(const string &Code , const string &Keyword);
int GetCodeFromKeyword(const string &Keyword);
bool AcceptSucceedingSpecialChar(const string &Text, char C);

#endif
