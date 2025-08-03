/*
 * This will trim the content
 */
#include "Reader.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "Characters.h"
#include "FileData.h"


bool Reader::ReadFile(CFFile* f, bool trimspecialchar){
    //
    TrimSpecialChar = trimspecialchar;
    FileToRead = f;
    char c;
    while (f->FileContent.get(c)) {
        GetCodes(c);
    }
    return true;
}

enum class ProcessType: short{
    Text, NumberAndByte, Comment, Char, None
};

ProcessType p = ProcessType::None;

void Reader::GetCodes(const char &c){
    // Transfer the whole file into symbols and words
    static string CurrentText;
    static string Note;
    static int CurrentLine = 0;

Beginning:
    if(p == ProcessType::Comment){
        if(c == '\n'){
            p = ProcessType::None;
            CurrentLine++;
        }
        return;
    }

    if(p == ProcessType::Text){
        CurrentText += c;
        if(c == '\\'){
            //escape character
            Note = "escape";
        }else if(c == CurrentText[0]){
            if(Note == "escape"){
                Note.clear();
            }else{
                Note.clear();
                PushSymbol(CurrentText);
            }
        }else{
            Note.clear();
        }
        return;
    }

    if(p == ProcessType::Char){
        if(AcceptSucceedingSpecialChar(CurrentText, c)){
            CurrentText += c;
            return;
        }else{
            int Result = PushSymbol(CurrentText);
            if(Result == 31){
                p = ProcessType::Comment;
                goto Beginning;
            }
        }
    }

    if(c == '\n'){
        //New line
        CurrentLine ++;
        PushSymbol(CurrentText);
        CurrentText += c;
        PushSymbol(CurrentText);
    }else if(c == ' ' || c == '\t'){
        //Blank
        PushSymbol(CurrentText);
    }else if(IsNumberChar(c)){
        //Number
        CurrentText += c;
    }else if(IsAlphabetChar(c)){
        //Alphabet
        CurrentText += c;
    }else{
        //Special char
        if(TrimSpecialChar){
            if(c == '\'' || c == '\"' ){
                PushSymbol(CurrentText);
                p = ProcessType::Text;
            }else{
                PushSymbol(CurrentText);
                p = ProcessType::Char;
            }
        }else{
            if(c == '\'' || c == '\"' ){
                PushSymbol(CurrentText);
                p = ProcessType::Text;
            }
        }
        CurrentText += c;
    }
}


int Reader::PushSymbol(string &Symbol){
    p = ProcessType::None;
    if(! Symbol.empty()){
        int c = GetCodeFromKeyword(Symbol);

        cout << (int)c << " "<<Symbol <<endl;
        pair<int, string> p;
        p.first = c;
        p.second = Symbol;
        FileToRead->Codes.push_back(p);
        Symbol.clear();
        return c;
    }
    return -1;
}
