#include "Codes.h"
#include "../Tools.h"

#include <cstdio>
#include <map>
#include <vector>


vector<LangTemp> LangTemp::Templates = {};

vector<char> OtherAcceptableCodeChar = {'_'};

LangTemp *LangTemp::NewLangTemp(const string& langname){
    //check if Language already exist
    LangTemp *temp = LangTemp::GetLangTemp(langname);
    if(temp != nullptr){
        return temp;//exist
    };

    LangTemp::Templates.emplace_back();
    temp = &LangTemp::Templates[LangTemp::Templates.size()-1];
    temp->LangName = langname;

    return temp;
}

LangTemp *LangTemp::GetLangTemp(const string& langname){
    for(LangTemp &temp: LangTemp::Templates){
        if (temp.LangName == langname) {
            //printf("template found for %s\n", langname.c_str());
            return &temp;
        }
    }
    //printf("template not found for %s\n", langname.c_str());
    return nullptr;
}

string& LangTemp::GetMultilineCommentEndSym(){
    return MulCmtEndSymb;
}

bool LangTemp::IsAcceptableCodeNameChar(char c){
    if(IsAlphabetChar(c)){
        return true;
    }else if(IsNumberChar(c)){
        return true;
    }else if(CharVecContains(OtherAcceptableCodeChar, c)){
        return true;
    }
    return false;
}

bool LangTemp::IsAcceptatbleNumByteChar(char c){
    if(IsNumberChar(c)){
        return true;
    }else if(c == '.'){
        return true;
    }

    return false;
}

void LangTemp::AddCFCode(const string &Keyword, unsigned short Code){
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

void LangTemp::AddCustKeyword(const string &Code , const string &Keyword){
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

int LangTemp::GetCodeFromKeyword(const string &Keyword){
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

bool LangTemp::AcceptSucceedingSpecialChar(const string &Text, char C){
    for(pair<string, int> s: CharKeywords){
        if(StartWith(s.first,Text)){
            if(s.first[Text.length()] == C){
                return true;
            }
        }
    }
    return false;
}
