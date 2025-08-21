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


bool Tokenizer::TokenizeFile(CFFile* f, bool trimspecialchar){
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


void Tokenizer::GetCodes(char c){
    // Transfer the whole file into symbols and words
    CurrentReadingPos++;// started from 1

    // Handle end of file
    if (c == 0) {
        CodeEndPos = CurrentReadingPos - 1;
        PushSymbol();
        return;
    }

    // Track line numbers
    if(c == '\n'){
        CurrentLine++;
    }

    // Process character based on current state
    if (ProcessCurrentState(c)) {
        return;
    }

    // Handle new characters when not in a specific state
    ProcessNewCharacter(c);
}

bool Tokenizer::ProcessCurrentState(char c) {
    switch(CodeType) {
        case BasicCodeTypes::SINGLELINECOMMENT:
            return ProcessSingleLineComment(c);
        case BasicCodeTypes::MULTILINECOMMENT:
            return ProcessMultiLineComment(c);
        case BasicCodeTypes::STRING:
            return ProcessTextState(c);
        case BasicCodeTypes::NUM:
            return ProcessNumberState(c);
        case BasicCodeTypes::NORMALCODE:
            return ProcessNormalCodeState(c);
        case BasicCodeTypes::CHAR:
            return ProcessCharState(c);
        default:
            return false; // Continue to process as new character
    }
}

bool Tokenizer::ProcessSingleLineComment(char c) {
    if(c == '\n'){
        CodeEndPos = CurrentReadingPos - 1;
        PushSymbol();
        CodeType = BasicCodeTypes::NONE;
        ProcessNewline(c);
    } else {
        CurrentCode += c;
    }
    return true;
}

bool Tokenizer::ProcessMultiLineComment(char c) {
    CurrentCode += c;
    if(EndWith(CurrentCode, GetMultilineCommentEndSym())){
        CodeEndPos = CurrentReadingPos;
        PushSymbol();
        CodeType = BasicCodeTypes::NONE;
    }
    return true;
}

bool Tokenizer::ProcessTextState(char c) {
    CurrentCode += c;
    if(c == '\\'){
        // escape character
        isEscapeNext = true;
    } else if(c == CurrentCode[0]){
        if(isEscapeNext){
            isEscapeNext = false;
        } else {
            CodeEndPos = CurrentReadingPos;
            PushSymbol();
        }
    } else {
        isEscapeNext = false;
    }
    return true;
}

bool Tokenizer::ProcessNumberState(char c) {
    if(IsAcceptatbleNumByteChar(c)){
        CurrentCode += c;
        return true;
    }
    // Number ended, continue processing this character
    return false;
}

bool Tokenizer::ProcessNormalCodeState(char c) {
    if(IsAcceptableCodeNameChar(c)){
        CurrentCode += c;
        return true;
    }
    // Code name ended, continue processing this character
    return false;
}

bool Tokenizer::ProcessCharState(char c) {
    if(AcceptSucceedingSpecialChar(CurrentCode, c)){
        CurrentCode += c;
        return true;
    } else {
        int result = GetCodeFromKeyword(CurrentCode);
        if(result == BasicCodeTypes::SINGLELINECOMMENT){
            CodeType = BasicCodeTypes::SINGLELINECOMMENT;
            ProcessSingleLineComment(c);
        } else if(result == BasicCodeTypes::MULTILINECOMMENTSTART){
            CodeType = BasicCodeTypes::MULTILINECOMMENT;
            ProcessMultiLineComment(c);
        } else {
            CodeEndPos = CurrentReadingPos - 1;
            PushSymbol();
            ProcessNewCharacter(c);
        }
        return true;
    }
}

void Tokenizer::ProcessNewCharacter(char c) {
    if(c == '\n'){
        ProcessNewline(c);
    } else if(c == ' ' || c == '\t'){
        ProcessWhitespace();
    } else if(IsNumberChar(c)){
        ProcessNumberStart(c);
    } else if(IsAlphabetChar(c)){
        ProcessAlphabetStart(c);
    } else {
        ProcessSpecialChar(c);
    }
}

void Tokenizer::ProcessNewline(char c) {
    CodeEndPos = CurrentReadingPos - 1;
    PushSymbol();
    CodeStartPos = CurrentReadingPos;
    CurrentCode += c;
    CodeEndPos = CurrentReadingPos;
    PushSymbol();
}

void Tokenizer::ProcessWhitespace() {
    CodeEndPos = CurrentReadingPos - 1;
    PushSymbol();
}

void Tokenizer::ProcessNumberStart(char c) {
    CurrentCode += c;
    CodeType = BasicCodeTypes::NUM;
    CodeStartPos = CurrentReadingPos;
}

void Tokenizer::ProcessAlphabetStart(char c) {
    CurrentCode += c;
    CodeType = BasicCodeTypes::NORMALCODE;
    CodeStartPos = CurrentReadingPos;
}

void Tokenizer::ProcessSpecialChar(char c) {
    if(c == '\'' || c == '\"'){
        CodeEndPos = CurrentReadingPos - 1;
        PushSymbol();
        CodeType = BasicCodeTypes::STRING;
        CurrentCode += c;
        CodeStartPos = CurrentReadingPos;
    } else if(TrimSpecialChar){
        CodeEndPos = CurrentReadingPos - 1;
        PushSymbol();
        CodeType = BasicCodeTypes::CHAR;
        CurrentCode += c;
        CodeStartPos = CurrentReadingPos;
    } else {
        CurrentCode += c;
        CodeStartPos = CurrentReadingPos;
    }
}


int Tokenizer::PushSymbol(){
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
        CodeType = BasicCodeTypes::NONE;
        return intcode;
    }
    return -1;
}
