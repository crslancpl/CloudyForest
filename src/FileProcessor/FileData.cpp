#include "FileData.h"
#include "../FileManager/FileManager.h"
#include "../Tools.h"
#include "../SectionData.h"
#include "Interpret.h"
#include "Tokenizer.h"
#include "Codes.h"
#include <cstdio>
#include <vector>
#include <memory>

/* CFFile */
vector<shared_ptr<CFFile>> ProcessedFile = {};

void ClearProcessedFile(){
    ProcessedFile.clear();
}

void ProcessFile(const string &filepath,const string &lang, CFFileType filetype){
    if(CheckIfProcessed(filepath)){
        // Processed
        printf("%s processed\n", filepath.c_str());
        return;
    }
    shared_ptr<CFFile> cf = make_shared<CFFile>();
    ProcessedFile.push_back(cf);
    cf->Language = lang;
    cf->Get(filepath, filetype);
}

bool CheckIfProcessed(const string &filepath){
    for (shared_ptr<CFFile> file : ProcessedFile) {
        if(file->FilePath == filepath){
            return true;
        }
    }
    return false;
}

shared_ptr<CFFile> FindCFFile(const string &filename){
    for (shared_ptr<CFFile>& file : ProcessedFile) {
        if(file->FilePath == filename){
            return file;
        }
    }
    return NULL;
}

void CFFile::Get(const string &filepath, CFFileType filetype){
    FilePath = filepath;
    FileType = filetype;
    RequestFile(filepath.c_str());
}

void CFFile::Read(){
    Tokenizer tokenizer;
    //null for unknow template and project file

    switch (FileType) {
        case CFFileType::Project:
            SetDir(GetParentDir(this->FilePath));
            tokenizer.TokenizeFile(this,nullptr);
            CFProjectInterp(this);
            break;
        case CFFileType::Template:
            if(LangTemp::GetLangTemp(this->Language) != nullptr){
                return;
            }
            tokenizer.TokenizeFile(this,nullptr);
            CFTemplateInterp(this);
            break;
        case CFFileType::SourceCode:
            tokenizer.TokenizeFile(this, LangTemp::GetLangTemp(Language));
            CFLangInterp(this);
            break;
        default:
            break;
    }
}
