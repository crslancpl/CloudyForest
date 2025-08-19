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
#include <vector>

#include "Codes.h"
#include "FileData.h"
#include "../Tools.h"


bool Reader::ReadFile(CFFile* f, bool trimspecialchar){
    //
    TrimSpecialChar = trimspecialchar;
    FileToRead = f;
    char c;
    while (f->FileContent.get(c)) {
        GetCodes(c);
    }
    GetCodes(0);
    return true;
}

enum class ProcessType: short{
    Text, NumberAndByte, SingleLineComment, MultiLineComment, Char, NormalCode, None
};

ProcessType p = ProcessType::None;

void Reader::GetCodes(const char &c){
    // Transfer the whole file into symbols and words
    static string Note;

    CurrentReadingPos ++;// started from 1
Beginning:
    if (c == 0) {
        CodeEndPos = CurrentReadingPos-1;
        PushSymbol();
    }
    if(c == '\n'){
        CurrentLine++;
    }

    if(p == ProcessType::SingleLineComment){
        if(c == '\n'){
            p = ProcessType::None;
        }
        return;
    }else if(p == ProcessType::MultiLineComment){
        //if(GetCodeFromKeyword() == MULTILINECOMMENTEND){
            //
            //}
    }else if(p == ProcessType::Text){
        CurrentCode += c;
        if(c == '\\'){
            //escape character
            Note = "escape";
        }else if(c == CurrentCode[0]){
            if(Note == "escape"){
                Note.clear();
            }else{
                Note.clear();
                CodeEndPos = CurrentReadingPos;
                PushSymbol();
            }
        }else{
            Note.clear();
        }
        return;
    }else if(p == ProcessType::NumberAndByte){
        if(IsAcceptatbleNumByteChar(c)){
            CurrentCode += c;
            return;
        }
    }else if(p== ProcessType::NormalCode){
        if(IsAcceptableCodeNameChar(c)){
            CurrentCode += c;
            return;
        }
    }else if(p == ProcessType::Char){
        if(AcceptSucceedingSpecialChar(CurrentCode, c)){
            CurrentCode += c;
            return;
        }else{
            CodeEndPos = CurrentReadingPos-1;
            int Result = PushSymbol();
            if(Result == 31){
                p = ProcessType::SingleLineComment;
                goto Beginning;
            }
        }

    }



    if(c == '\n'){
        //New line
        CodeEndPos = CurrentReadingPos -1;
        PushSymbol();
        CodeStartPos = CodeEndPos = CurrentReadingPos;
        CurrentCode += c;
        PushSymbol();
    }else if(c == ' ' || c == '\t'){
        //Blank
        CodeEndPos = CurrentReadingPos -1;
        PushSymbol();
    }else if(IsNumberChar(c)){
        //Number
        CurrentCode += c;
        p = ProcessType::NumberAndByte;
    }else if(IsAlphabetChar(c)){
        //Alphabet
        CurrentCode += c;
        p = ProcessType::NormalCode;
    }else{
        //Special char
        if(TrimSpecialChar){
            if(c == '\'' || c == '\"' ){
                CodeEndPos = CurrentReadingPos-1;
                PushSymbol();
                p = ProcessType::Text;
            }else{
                CodeEndPos = CurrentReadingPos -1;
                PushSymbol();
                p = ProcessType::Char;
            }
        }else{
            if(c == '\'' || c == '\"' ){
                PushSymbol();
                p = ProcessType::Text;
            }
        }
        CurrentCode += c;
    }
    CodeStartPos = CurrentReadingPos;
}


int Reader::PushSymbol(){
    p = ProcessType::None;
    if(! CurrentCode.empty()){
        int intcode = GetCodeFromKeyword(CurrentCode);

        //cout << (int)intcode << " "<<CurrentCode <<endl;
        CFCode Code;
        Code.IntCode = intcode;
        Code.Content = CurrentCode;
        Code.StartPos = CodeStartPos;
        Code.EndPos = CodeEndPos;
        Code.CodeType = CodeType;
        FileToRead->Codes.push_back(Code);

        CurrentCode.clear();
        CodeType = CFCodeType::Unknown;
        return intcode;
    }
    return -1;
}


vector<char> OtherAcceptableCodeChar = {};

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
