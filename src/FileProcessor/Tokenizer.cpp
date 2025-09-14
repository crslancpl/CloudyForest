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


bool Tokenizer::TokenizeFile(CFFile* f, LangTemp* temp){
    /*
     * Convert the whole text to tokens (individual symbols)
     */
    FileToRead = f;

    Template = temp;
    f->LanguageTemplate = temp;
    char c;
    if(Template == nullptr){
        while (f->FileContent.get(c)) {
            TokenizeTemplate(c);
        }
        TokenizeTemplate(0);
    }else{
        TrimSpecialChar = temp->TrimSpecialChar;
        while (f->FileContent.get(c)) {
            TokenizeCodes(c);
        }
        TokenizeCodes(0);

    }
    return true;
}

void Tokenizer::TokenizeTemplate(char c){
    if(c == ' ' || c == '\t' || c == '\n' || c =='\r'){
        if(!CurrentCode.empty()){
            CFCode code;
            code.CodeType = NORMALCODE;
            code.Content = CurrentCode;
            FileToRead->Codes.push_back(code);
            CurrentCode.clear();
        }
    }else{
        CurrentCode+=c;
    }
}

void Tokenizer::TokenizeCodes(char c){
    /*
     * Transfer the whole file into symbols and words
     */

    CurrentReadingPos++;// started from 1

    // Handle end of file
    if (c == 0) {
        MarkTokenEnd(0);
        PushSymbol();
        return;
    }

    if (ProcessCurrentState(c)) {
        // Process character based on current state
    }else{
        // Handle new characters when not in a specific state
        ProcessNewCharacter(c);
    }



    // Track line numbers
    if(c == '\n'){
        CurrentLine++;
        CurrentLineCharPos = 1;// started from 1
    }else{
        CurrentLineCharPos++;
    }
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
        MarkTokenEnd(-1);
        PushSymbol();
        CodeType = BasicCodeTypes::NEWLINE;
        ProcessNewline(c);
    } else {
        CurrentCode += c;
    }
    return true;
}

bool Tokenizer::ProcessMultiLineComment(char c) {
    CurrentCode += c;
    if(EndWith(CurrentCode, Template->GetMultilineCommentEndSym())){
        MarkTokenEnd(0);
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
            MarkTokenEnd(0);
            PushSymbol();
        }
    } else {
        isEscapeNext = false;
    }
    return true;
}

bool Tokenizer::ProcessNumberState(char c) {
    if(Template->IsAcceptatbleNumByteChar(c)){
        CurrentCode += c;
        return true;
    }
    // Number ended, continue processing this character
    return false;
}

bool Tokenizer::ProcessNormalCodeState(char c) {
    if(Template->IsAcceptableCodeNameChar(c)){
        CurrentCode += c;
        return true;
    }
    // Code name ended, continue processing this character
    return false;
}

bool Tokenizer::ProcessCharState(char c) {
    if(Template->AcceptSucceedingSpecialChar(CurrentCode, c)){
        CurrentCode += c;
        return true;
    }else {
        int result = Template->GetCodeFromKeyword(CurrentCode);
        if(result == BasicCodeTypes::SINGLELINECOMMENT){
            CodeType = BasicCodeTypes::SINGLELINECOMMENT;
            ProcessSingleLineComment(c);
        } else if(result == BasicCodeTypes::MULTILINECOMMENTSTART){
            CodeType = BasicCodeTypes::MULTILINECOMMENT;
            ProcessMultiLineComment(c);
        } else if(result == BasicCodeTypes::STRING){
            CodeType = BasicCodeTypes::STRING;
            ProcessTextState(c);
        } else {
            MarkTokenEnd(-1);
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
    } else if(Template->IsAcceptableCodeNameChar(c)){
        ProcessAlphabetStart(c);
    } else {
        ProcessSpecialChar(c);
    }
}

void Tokenizer::ProcessNewline(char c) {
    MarkTokenEnd(-1);
    PushSymbol();
    MarkTokenStart();
    CurrentCode += c;
    MarkTokenEnd(0);
    PushSymbol();
}

void Tokenizer::ProcessWhitespace() {
    MarkTokenEnd(0);
    PushSymbol();
}

void Tokenizer::ProcessNumberStart(char c) {
    CurrentCode += c;
    CodeType = BasicCodeTypes::NUM;
    MarkTokenStart();
}

void Tokenizer::ProcessAlphabetStart(char c) {
    CurrentCode += c;
    CodeType = BasicCodeTypes::NORMALCODE;
    MarkTokenStart();
}

void Tokenizer::ProcessSpecialChar(char c) {
    if(TrimSpecialChar){
        MarkTokenEnd(-1);
        PushSymbol();
        CodeType = BasicCodeTypes::CHAR;
        CurrentCode += c;
        MarkTokenStart();
    } else {
        CurrentCode += c;
        MarkTokenStart();
    }
}

void Tokenizer::MarkTokenStart(){
    CodeStartPos = CurrentReadingPos;
    CodeStartLine = CurrentLine;
    CodeStartLinePos = CurrentLineCharPos;
}
void Tokenizer::MarkTokenEnd(int difference){
    CodeEndPos = CurrentReadingPos + difference;
    CodeEndLine = CurrentLine;
    CodeEndLinePos = CurrentLineCharPos;
}

int Tokenizer::PushSymbol(){
    if(! CurrentCode.empty()){

        int intcode = Template->GetCodeFromKeyword(CurrentCode);

        //cout << (int)intcode << " "<<CurrentCode <<endl;
        CFCode Code;
        Code.IntCode = intcode;
        Code.Content = CurrentCode;
        Code.CodeType = CodeType;

        //Get position form absolute position
        Code.StartPos = CodeStartPos;
        Code.EndPos = CodeEndPos;

        //Get position from line and relative position
        Code.StartLine = CodeStartLine;
        Code.StartLinePos = CodeStartLinePos;
        Code.EndLine = CodeEndLine;
        Code.EndLinePos = CodeEndLinePos;
        Code.TextLength = CurrentCode.length();

        //printf("%s %i", Code.Content.c_str(), intcode);

        FileToRead->Codes.push_back(Code);

        CurrentCode.clear();
        CodeType = BasicCodeTypes::NONE;
        return intcode;
    }
    return -1;
}
