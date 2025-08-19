#include "Interpret.h"

#include "../SectionData.h"
#include "Codes.h"
#include "FileData.h"
#include "../CFEmbed.h"

#include <cstdio>
#include <memory>
#include <utility>
#include <string.h>

enum class ProjectNote{
    NONE,TAG,NAME,VERSION,DESCRIPTION,LANGUAGE,TEMPLATE,ENTRYFILE
};

void CFProjectInterp(CFFile *F){
    ProjectNote n;
    for (CFCode& code: F->Codes) {
        printf("%s", code.Content.c_str());
        if(code.Content == "#Template"){
            n = ProjectNote::TEMPLATE;
        }else if(code.Content == "#Name"){
            n = ProjectNote::NAME;
        }else if(code.Content == "#Version"){
            //
        }else if(code.Content == "#Language"){
            n = ProjectNote::TEMPLATE;
        }else if(code.Content == "#EntryFile"){
            n = ProjectNote::ENTRYFILE;
        }else if(n == ProjectNote::TEMPLATE){
            CFFile::ProcessFile("LangTemp/"+ code.Content + "/Template.txt", CFFileType::Template);
            n = ProjectNote::NONE;
        }else if(n == ProjectNote::ENTRYFILE){
            CFFile::ProcessFile("TestInputFile/"+code.Content, CFFileType::SourceCode);
            n = ProjectNote::NONE;
        }
    }
}

enum class TemplateNote{
    OTHER, TYPE, DEF, KEYWORD
};

void CFTemplateInterp(CFFile* F){
    TemplateNote note;
    for (CFCode& code : F->Codes) {
        if(code.Content == "\n")continue;
        if(code.Content == "#Type"){
            //indicates type
            note = TemplateNote::TYPE;
        }else if(code.Content == "#Keyword"){
            note = TemplateNote::KEYWORD;
        }else if(StartWith(code.Content, "#")){
            note = TemplateNote::OTHER;
        }else if (StartWith(code.Content, "{") && EndWith(code.Content, "}")) {
            //variable
        }else if(note == TemplateNote::TYPE){
            AddCFCode(code.Content, BasicCodeTypes::TYPE);
        }else if(note == TemplateNote::KEYWORD){
            AddCFCode(code.Content, BasicCodeTypes::KEYWORD);
        }
    }
}

Message mes;
Highlight hl;
void CFLangInterp(CFFile *F){
    for (CFCode& code : F->Codes) {
        if(GetAppType()== AppType::Embed){


            hl.startpos = code.StartPos;
            hl.endpos = code.EndPos;

            hl.file = strdup(F->FilePath.c_str());
            mes.Type = MessageType::DRAW;
            if(GetCodeFromKeyword(code.Content) == TYPE){
                hl.type = t::CF_TYPE;
            }else if(GetCodeFromKeyword(code.Content) == KEYWORD){
                hl.type = t::CF_KEYWORD;
            }else{
                hl.type = t::CF_NONE;
            }
            mes.Data = &hl;
            emb_Send_Message(&mes);
        }else if(GetAppType()== AppType::Compiler){
            if(GetCodeFromKeyword(code.Content) == TYPE){
            }else if(GetCodeFromKeyword(code.Content) == KEYWORD){
            }
        }

    }
}
