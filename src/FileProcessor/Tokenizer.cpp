/*
 * This will trim the content into words
 */
#include "Tokenizer.h"

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


void Reader::GetCodes(char c){
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

    if(CodeType == CFCodeType::SingleLineComment){
        if(c == '\n'){
            CodeEndPos = CurrentReadingPos -1;
            PushSymbol();
            CodeType = CFCodeType::None;
        }else{
            CurrentCode += c;
        }
        return;
    }else if(CodeType == CFCodeType::MultiLineComment){
        CurrentCode += c;
        if(EndWith(CurrentCode, GetMultilineCommentEndSym())){
            CodeEndPos = CurrentReadingPos;
            PushSymbol();
            CodeType = CFCodeType::None;
        }
        return;
    }else if(CodeType == CFCodeType::Text){
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
    }else if(CodeType == CFCodeType::NumberAndByte){
        if(IsAcceptatbleNumByteChar(c)){
            CurrentCode += c;
            return;
        }
    }else if(CodeType== CFCodeType::NormalCode){
        if(IsAcceptableCodeNameChar(c)){
            CurrentCode += c;
            return;
        }
    }else if(CodeType == CFCodeType::Char){
        if(AcceptSucceedingSpecialChar(CurrentCode, c)){
            CurrentCode += c;
            return;
        }else{
            int Result = GetCodeFromKeyword(CurrentCode);
            if(Result == BasicCodeTypes::SINGLELINECOMMENT){
                CodeType = CFCodeType::SingleLineComment;
                goto Beginning;
            }else if(Result == BasicCodeTypes::MULTILINECOMMENTSTART){
                CodeType = CFCodeType::MultiLineComment;
                goto Beginning;
            }else{
                CodeEndPos = CurrentReadingPos-1;
                PushSymbol();
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
        CodeType = CFCodeType::NumberAndByte;
    }else if(IsAlphabetChar(c)){
        //Alphabet
        CurrentCode += c;
        CodeType = CFCodeType::NormalCode;
    }else{
        //Special char
        if(c == '\'' || c == '\"' ){
            CodeEndPos = CurrentReadingPos-1;
            PushSymbol();
            CodeType = CFCodeType::Text;
        }else if(TrimSpecialChar){
            CodeEndPos = CurrentReadingPos -1;
            PushSymbol();
            CodeType = CFCodeType::Char;
        }
        CurrentCode += c;
    }
    CodeStartPos = CurrentReadingPos;
}


int Reader::PushSymbol(){
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
        CodeType = CFCodeType::None;
        return intcode;
    }
    return -1;
}
