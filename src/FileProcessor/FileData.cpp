#include "FileData.h"
#include "../FileManager/FileManager.h"
#include "Interpret.h"
#include "Tokenizer.h"
#include <cstdio>
#include <vector>
#include <memory>

/* CFFile */
vector<shared_ptr<CFFile>> CFFile::ProcessedFile = {};

void CFFile::ProcessFile(const string &filepath, CFFileType filetype){
    if(CheckIfProcessed(filepath)){
        // Processed
        printf("%s processed\n", filepath.c_str());
        return;
    }

    shared_ptr<CFFile> cf = make_shared<CFFile>();
    ProcessedFile.push_back(cf);
    cf->Get(filepath, filetype);
}

bool CFFile::CheckIfProcessed(const string &filepath){
    for (shared_ptr<CFFile> file : ProcessedFile) {
        if(file->FilePath == filepath){
            return true;
        }
    }
    return false;
}

shared_ptr<CFFile> CFFile::FindCFFile(const string &filename){
    for (shared_ptr<CFFile> file : ProcessedFile) {
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
    Reader r;
    switch (FileType) {
        case CFFileType::Project:
            r.ReadFile(this,false);
            CFProjectInterp(this);
            break;
        case CFFileType::Template:
            r.ReadFile(this,false);
            CFTemplateInterp(this);
            break;
        case CFFileType::SourceCode:
            r.ReadFile(this, true);
            CFLangInterp(this);
            break;
        default:
            break;
    }
}
