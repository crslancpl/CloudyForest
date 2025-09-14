/*
 * Code.h and Code.cpp are holding the template of the language
 *
 */
#ifndef CODES
#define CODES

#include <string>
#include <map>
#include <vector>

using namespace std;

enum BasicCodeTypes: int{
    NONE, SINGLELINECOMMENT, MULTILINECOMMENTSTART, MULTILINECOMMENTEND, MULTILINECOMMENT,
    TYPE, NUM, BYTE, STRING, KEYWORD, CHAR, NORMALCODE, FUNCTION,
    TAGS, TAGSYMBOL, VALUE, NEWLINE, MODIFIER, AREAMODIFIER
};

class LangTemp{
    public:

    static vector<LangTemp> Templates;
    static LangTemp* GetLangTemp(const string& langname);//null if not loaded
    static LangTemp* NewLangTemp(const string& langname);//create new langtemp in Templates
    static void RemoveLangTemp(const string& langname);//remove the langtemp in Templates

    vector<char> AcceptableCharInName;

    string LangName;
    bool TrimSpecialChar = true; //default true
    string MulCmtEndSymb; //multiline comment end symbol
    map< string, int> KeywordsTypes;//all
    map< string, int> Keywords;//alphabets
    map< string, int> CharKeywords;//special characters

    string& GetMultilineCommentEndSym();
    void AddAcceptableCharInName(char c);
    void AddCFCode(const string &Keyword, unsigned short Code);
    void AddCustKeyword(const string &Code , const string &Keyword);
    int GetCodeFromKeyword(const string &Keyword);
    bool AcceptSucceedingSpecialChar(const string &Text, char C);
    bool IsAcceptableCodeNameChar(char c);
    bool IsAcceptatbleNumByteChar(char c);
};

#endif
