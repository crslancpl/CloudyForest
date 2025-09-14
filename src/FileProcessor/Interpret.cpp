#include "Interpret.h"

#include "../SectionData.h"
#include "Codes.h"
#include "FileData.h"
#include "../CFEmbed.h"
#include "../Tools.h"

#include <cstdio>
#include <memory>
#include <type_traits>
#include <utility>
#include <string.h>
#include <unordered_map>
#include <functional>


/*
 * Project
 */
enum class ProjectNote{
    NONE,TAG,NAME,VERSION,DESCRIPTION,LANGUAGE,TEMPLATE,ENTRYFILE
};

void CFProjectInterp(CFFile *F){
    ProjectNote currentNote = ProjectNote::NONE;

    // Map of directive handlers
    std::unordered_map<std::string, std::function<void()>> directiveHandlers = {
        {"#Template", [&]() { currentNote = ProjectNote::TEMPLATE; }},
        {"#Name", [&]() { currentNote = ProjectNote::NAME; }},
        {"#Version", [&]() { currentNote = ProjectNote::VERSION; }},
        {"#Language", [&]() { currentNote = ProjectNote::LANGUAGE; }},
        {"#EntryFile", [&]() { currentNote = ProjectNote::ENTRYFILE; }},
    };

    for (CFCode& code : F->Codes) {
        // Handle directives
        auto it = directiveHandlers.find(code.Content);
        if (it != directiveHandlers.end()) {
            it->second();
            continue;
        }

        // Handle content based on current note
        switch (currentNote) {
            case ProjectNote::TEMPLATE:
                F->Language = code.Content;
                ProcessFile("syntax/" + code.Content + ".txt", code.Content ,CFFileType::Template);
                currentNote = ProjectNote::NONE;
                break;
            case ProjectNote::ENTRYFILE:
                ProcessFile(GetDir() + code.Content, F->Language ,CFFileType::SourceCode);
                currentNote = ProjectNote::NONE;
                break;
            default:
                break;
        }
    }
}


/*
 * Template
 */
enum class TemplateNote{
    OTHER, TYPE, DEF, KEYWORD, SINGCMT, MULTCMTSTART, MULTCMTEND,
    FUNCTION, NEWTYPE, NEWFUNC, TAGSYMBOL, TAGS, TEXTSYMBOL, VALUE,
    ACCEPTCHARINNAME, AREAMODIFIER, MODIFIER
};

void CFTemplateInterp(CFFile* F){
    // Map directive strings to note types
    static std::unordered_map<std::string, TemplateNote> directiveMap = {
        {"#Type", TemplateNote::TYPE},
        {"#Keyword", TemplateNote::KEYWORD},
        {"#AreaModifier", TemplateNote::AREAMODIFIER},
        {"#Modifier", TemplateNote::MODIFIER},
        {"#SLCmt", TemplateNote::SINGCMT},
        {"#MLCmtS", TemplateNote::MULTCMTSTART},
        {"#MLCmtE", TemplateNote::MULTCMTEND},
        {"#TRIMCHAR", TemplateNote::OTHER},
        {"#Function",TemplateNote::FUNCTION},
        {"#NewType",  TemplateNote::NEWTYPE},
        {"#NewFunction",TemplateNote::NEWFUNC},
        {"#TagSymbol", TemplateNote::TAGSYMBOL},
        {"#Tags", TemplateNote::TAGS},
        {"#TextSymbol", TemplateNote::TEXTSYMBOL},
        {"#Value", TemplateNote::VALUE},
        {"#AcceptCharInName", TemplateNote::ACCEPTCHARINNAME}
    };

    // Map note types to their corresponding BasicCodeTypes
    static std::unordered_map<TemplateNote, BasicCodeTypes> noteToCodeType = {
        {TemplateNote::TYPE, BasicCodeTypes::TYPE},
        {TemplateNote::KEYWORD, BasicCodeTypes::KEYWORD},
        {TemplateNote::MODIFIER, BasicCodeTypes::MODIFIER},
        {TemplateNote::AREAMODIFIER, BasicCodeTypes::AREAMODIFIER},
        {TemplateNote::SINGCMT, BasicCodeTypes::SINGLELINECOMMENT},
        {TemplateNote::MULTCMTSTART, BasicCodeTypes::MULTILINECOMMENTSTART},
        {TemplateNote::MULTCMTEND, BasicCodeTypes::MULTILINECOMMENTEND},
        {TemplateNote::TAGSYMBOL, BasicCodeTypes::TAGSYMBOL},
        {TemplateNote::TAGS, BasicCodeTypes::TAGS},
        {TemplateNote::TEXTSYMBOL, BasicCodeTypes::STRING},
        {TemplateNote::VALUE, BasicCodeTypes::VALUE}
    };

    TemplateNote currentNote = TemplateNote::OTHER;
    LangTemp *temp = LangTemp::NewLangTemp(F->Language);

    for (CFCode& code : F->Codes) {
        if (code.Content == "\n" ) continue;
        if(code.Content == "#TrimSpecialCharacter"){
            temp->TrimSpecialChar = true;
            continue;
        }
        // Check if it's a directive
        auto dirIt = directiveMap.find(code.Content);
        if (dirIt != directiveMap.end()) {
            currentNote = dirIt->second;
            continue;
        }

        if (currentNote == TemplateNote::ACCEPTCHARINNAME) {
            temp->AddAcceptableCharInName(code.Content[0]);
            continue;
        }

        // Skip unknown directives and variables
        if (StartWith(code.Content, "{") && EndWith(code.Content, "}")) {
            currentNote = TemplateNote::OTHER;
            continue;
        }

        // Process content based on current note
        auto codeIt = noteToCodeType.find(currentNote);
        if (codeIt != noteToCodeType.end()){
            temp->AddCFCode(code.Content, codeIt->second);
            //printf("%s %i\n", code.Content.c_str(), codeIt->second);
        }else {
            // printf("unknown\n");
        }
    }
}



/*
 * normal
 */

Highlight hl;

void ProcessCompilerMode(CFFile* F) {
    LangTemp *temp = LangTemp::GetLangTemp(F->Language);

    for (CFCode& code : F->Codes) {
        int keywordType = temp->GetCodeFromKeyword(code.Content);

        //printf("code: %s type: %i line %i pos: %i\n", code.Content.c_str(), code.CodeType, code.StartLine, code.StartLinePos);
    }
    //printf("\n");
}

t GetHighlightType(const CFCode& code, LangTemp* temp) {
    int keywordType = temp->GetCodeFromKeyword(code.Content);
    if (keywordType == TYPE) return t::CF_TYPE;
    if (keywordType == KEYWORD) return t::CF_KEYWORD;
    if (keywordType == VALUE) return t::CF_VALUE;
    if (keywordType == MODIFIER || keywordType == AREAMODIFIER) return t::CF_MODIFIER;


    switch (code.CodeType) {
        case MULTILINECOMMENT: return t::CF_MULTCMT;
        case SINGLELINECOMMENT: return t::CF_SINGCMT;
        case STRING: return t::CF_TEXT;
        case FUNCTION: return t::CF_FUNCTIONNAME;
        case NUM: return t::CF_VALUE;
        case TAGSYMBOL:
        case TAGS:
            return t::CF_TAG;
        case CHAR: return CF_CHAR;
        case NEWLINE: return CF_NEWLINE;
        default: return t::CF_NONE;
    }
}

void ProcessEmbedMode(CFFile* F) {
    LangTemp *temp = LangTemp::GetLangTemp(F->Language);
    for (CFCode& code : F->Codes) {
        hl.startpos = code.StartPos;
        hl.endpos = code.EndPos;

        hl.startline = code.StartLine;
        hl.startlinepos = code.StartLinePos;
        hl.endline = code.EndLine;
        hl.endlinepos = code.EndLinePos;
        hl.textlength = code.TextLength;

        hl.file = strdup(F->FilePath.c_str());
        hl.type = GetHighlightType(code,temp);
        //printf("%i \n", hl.type);
        emb_Send_Message(MessageType::DRAW,&hl);
    }
}

void CFLangInterp(CFFile *F){
    AppType appType = GetAppType();
    FindTags(F);
    FindNewVar(F);
    //FindNewType(F);
    FindFunction(F);

    if (appType == AppType::Embed) {
        ProcessEmbedMode(F);
    } else if (appType == AppType::Compiler) {
        ProcessCompilerMode(F);
    }

}

void FindNewVar(CFFile *F){
    // not implemented
}

void FindTags(CFFile *F){
    bool IsTag = false;

    for (CFCode& code : F->Codes) {
        if(F->LanguageTemplate->GetCodeFromKeyword(code.Content) == BasicCodeTypes::TAGSYMBOL){
            code.CodeType = BasicCodeTypes::TAGSYMBOL;
            IsTag = true;
        }else if(IsTag && F->LanguageTemplate->GetCodeFromKeyword(code.Content) == BasicCodeTypes::TAGS){
            code.CodeType = BasicCodeTypes::TAGS;
            IsTag = false;
        }else{
            IsTag = false;
        }
    }
}

void FindNewType(CFFile *F){
    //
}

void FindFunction(CFFile *F){
    CFCode* PreviousCode = nullptr;
    for(CFCode& code: F->Codes){
        if(code.CodeType == NORMALCODE){
            PreviousCode = &code;
        }else if(code.Content == "(" && PreviousCode != nullptr){
            PreviousCode->CodeType = FUNCTION;
            PreviousCode = nullptr;
        }else{
            PreviousCode = nullptr;
        }
    }
}
