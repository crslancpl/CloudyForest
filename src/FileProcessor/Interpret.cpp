#include "Interpret.h"

#include "../SectionData.h"
#include "Codes.h"
#include "FileData.h"
#include "../CFEmbed.h"
#include "../Tools.h"

#include <cstdio>
#include <memory>
#include <utility>
#include <string.h>
#include <unordered_map>
#include <functional>

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
        {"#EntryFile", [&]() { currentNote = ProjectNote::ENTRYFILE; }}
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
                CFFile::ProcessFile("syntax/" + code.Content + ".txt", CFFileType::Template);
                currentNote = ProjectNote::NONE;
                break;
            case ProjectNote::ENTRYFILE:
                CFFile::ProcessFile(GetDir() + code.Content, CFFileType::SourceCode);
                currentNote = ProjectNote::NONE;
                break;
            default:
                break;
        }
    }
}

enum class TemplateNote{
    OTHER, TYPE, DEF, KEYWORD, SINGCMT, MULTCMTSTART, MULTCMTEND
};

void CFTemplateInterp(CFFile* F){
    TemplateNote currentNote = TemplateNote::OTHER;

    // Map directive strings to note types
    std::unordered_map<std::string, TemplateNote> directiveMap = {
        {"#Type", TemplateNote::TYPE},
        {"#Keyword", TemplateNote::KEYWORD},
        {"#SLCmt", TemplateNote::SINGCMT},
        {"#MLCmtS", TemplateNote::MULTCMTSTART},
        {"#MLCmtE", TemplateNote::MULTCMTEND},
        {"#TRIMCHAR", TemplateNote::OTHER},
    };

    // Map note types to their corresponding BasicCodeTypes
    std::unordered_map<TemplateNote, BasicCodeTypes> noteToCodeType = {
        {TemplateNote::TYPE, BasicCodeTypes::TYPE},
        {TemplateNote::KEYWORD, BasicCodeTypes::KEYWORD},
        {TemplateNote::SINGCMT, BasicCodeTypes::SINGLELINECOMMENT},
        {TemplateNote::MULTCMTSTART, BasicCodeTypes::MULTILINECOMMENTSTART},
        {TemplateNote::MULTCMTEND, BasicCodeTypes::MULTILINECOMMENTEND}
    };

    for (CFCode& code : F->Codes) {
        if (code.Content == "\n") continue;

        // Check if it's a directive
        auto dirIt = directiveMap.find(code.Content);
        if (dirIt != directiveMap.end()) {
            currentNote = dirIt->second;
            continue;
        }

        // Skip unknown directives and variables
        if (StartWith(code.Content, "#") && code.Content.length() > 1 ||
           (StartWith(code.Content, "{") && EndWith(code.Content, "}"))) {
            currentNote = TemplateNote::OTHER;
            continue;
        }

        // Process content based on current note
        auto codeIt = noteToCodeType.find(currentNote);
        if (codeIt != noteToCodeType.end()) {
            AddCFCode(code.Content, codeIt->second);
        }
    }
}

Message mes;
Highlight hl;

void ProcessCompilerMode(CFFile* F) {
    for (CFCode& code : F->Codes) {
        int keywordType = GetCodeFromKeyword(code.Content);

        if (keywordType == TYPE) {
            printf("%s Type\n", code.Content.c_str());
        } else if (keywordType == KEYWORD) {
            printf("%s Keyword\n", code.Content.c_str());
        }
    }
}

t GetHighlightType(const CFCode& code) {
    int keywordType = GetCodeFromKeyword(code.Content);

    if (keywordType == TYPE) return t::CF_TYPE;
    if (keywordType == KEYWORD) return t::CF_KEYWORD;

    switch (code.CodeType) {
        case CFCodeType::MultiLineComment: return t::CF_MULTCMT;
        case CFCodeType::SingleLineComment: return t::CF_SINGCMT;
        case CFCodeType::Text: return t::CF_TEXT;
        default: return t::CF_NONE;
    }
}

void ProcessEmbedMode(CFFile* F) {
    for (CFCode& code : F->Codes) {
        hl.startpos = code.StartPos;
        hl.endpos = code.EndPos;
        hl.file = strdup(F->FilePath.c_str());
        hl.type = GetHighlightType(code);

        mes.Type = MessageType::DRAW;
        mes.Data = &hl;
        emb_Send_Message(&mes);
    }
}

void CFLangInterp(CFFile *F){
    AppType appType = GetAppType();

    if (appType == AppType::Embed) {
        ProcessEmbedMode(F);
    } else if (appType == AppType::Compiler) {
        ProcessCompilerMode(F);
    }
}
