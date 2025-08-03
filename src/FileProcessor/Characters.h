#ifndef CHARACTERS
#define CHARACTERS

#include <string>

using namespace std;

bool IsNumberChar(char c);
bool IsAlphabetChar(char c);

bool StartWith(const string &Text, const string &Pattern);

void AddCFCode(const string &Keyword, unsigned short Code);
void AddCustKeyword(const string &Code , const string &Keyword);
int GetCodeFromKeyword(const string &Keyword);
bool AcceptSucceedingSpecialChar(const string &Text, char C);

#endif
