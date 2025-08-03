#include "Interpret.h"

#include "../SectionData.h"
#include "FileData.h"

#include <cstdio>
#include <memory>
#include <utility>

void CFTemplateInterp(CFFile* F){
    for (pair<int ,string> code : F->Codes) {
        //if()
    }
}

enum ProjectNote{
    NONE,TAG,NAME,VERSION,DESCRIPTION,LANGUAGE,TEMPLATE,ENTRYFILE
};

void CFProjectInterp(CFFile *F){
    ProjectNote n;
    for (pair<int, string> code: F->Codes) {
        if(code.second == "#Template"){

            n = TEMPLATE;
        }else if(code.second == "#Name"){
            n = NAME;
        }else if(code.second == "#Version"){
            //
        }else if(n == TEMPLATE){
            CFFile::ProcessFile(code.second, CFFileType::Template);
        }
    }
}
