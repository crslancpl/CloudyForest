#include "FileData.h"
#include "../FileManager/FileManager.h"
#include <cstdio>
#include <vector>
#include <memory>

/* CFFile */
vector<shared_ptr<CFFile>> CFFile::ProcessedFile = {};
map<string,CFFileType> CFFile::PendingFiles = {};

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

void CFFile::Get(const string &filepath, CFFileType filetype){
    FilePath = filepath;
    PendingFiles.insert({filepath,filetype});
    printf("%s getting\n", filepath.c_str());
    RequestFile(filepath.c_str());
}
