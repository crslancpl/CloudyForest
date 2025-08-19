#include "Codes.h"
#include "../Tools.h"

#include <cstdio>
#include <map>
#include <vector>

map< string, int> KeywordsTypes;
map< string, int> Keywords;
map< string, int> CharKeywords;
vector<char> OtherAcceptableCodeChar = {'_'};

string MulCmtEndSymb; //multiline comment end symbol
string& GetMultilineCommentEndSym(){
    return MulCmtEndSymb;
}

bool IsAcceptableCodeNameChar(char c){
    if(IsAlphabetChar(c)){
        return true;
    }else if(IsNumberChar(c)){
        return true;
    }else if(CharVecContains(OtherAcceptableCodeChar, c)){
        return true;
    }
    return false;
}

bool IsAcceptatbleNumByteChar(char c){
    if(IsNumberChar(c)){
        return true;
    }else if(c == '.'){
        return true;
    }

    return false;
}

bool IsNumberChar(char c){
    if(c >= 48 && c <= 57 ){
        return true;
    }
    return false;
}

bool IsAlphabetChar(char c){
    if(c >= 65 && c <= 90){
        return true;
    }

    if(c >= 97 && c <= 122){
        return true;
    }

    return false;
}

bool StartWith(const string &Text, const string &Pattern){
    if(Text.length() < Pattern.length()) return false;
    for(int i = 0; i < Pattern.length(); i++){
        if(Text[i] != Pattern[i])return false;
    }
    return true;
}

bool EndWith(const string &Text, const string &Pattern){
    if(Text.length() < Pattern.length()) return false;
    for(int i = 0; i < Pattern.length(); i++){
        if(Text[Text.length()-1 - i] != Pattern[Pattern.length()-1 - i])return false;
    }
    return true;
}

void AddCFCode(const string &Keyword, unsigned short Code){
    if(Code == BasicCodeTypes::MULTILINECOMMENTEND){
        MulCmtEndSymb = Keyword;
    }
    KeywordsTypes.emplace(Keyword, Code);
    if(IsAlphabetChar(Keyword[0])){
        // String keyword
        Keywords.emplace(Keyword, Code);
    }else{
        // Characters keyword
        CharKeywords.emplace(Keyword, Code);
    }
}

void AddCustKeyword(const string &Code , const string &Keyword){
    // Type can be either name or code of the keyword
    pair<string, int> P;

    P.first = Keyword;

    if(IsNumberChar(Code[0])){
        // The mapping is mapped by code instead of the name of keyword
        P.second = stoi(Code);
    }else{
        map<string, int>::iterator i = KeywordsTypes.find(Code);
        if(i != KeywordsTypes.end()){
            P.second = i ->second;
        }else{
            return;
        }
    }

    if(IsAlphabetChar(Keyword[0])){
        // String keyword
        Keywords.insert(Keywords.end(), P);
    }else{
        // Characters keyword
        CharKeywords.insert(CharKeywords.end(), P);
    }
}

int GetCodeFromKeyword(const string &Keyword){
    map<string, int>::iterator i;

    i = CharKeywords.find(Keyword);
    if(i != CharKeywords.end()){
        //It is a keyword
        return i->second;
    }
    //////
    i = Keywords.find(Keyword);
    if(i != Keywords.end()){
        //It is a keyword
        return i->second;
    }

    else return -1;
}

bool AcceptSucceedingSpecialChar(const string &Text, char C){
    for(pair<string, int> s: CharKeywords){
        if(StartWith(s.first,Text)){
            if(s.first[Text.length()] == C){
                return true;
            }
        }
    }
    return false;
}
