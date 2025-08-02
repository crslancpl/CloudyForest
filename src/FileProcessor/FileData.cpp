#include "FileData.h"
#include "../FileManager/FileManager.h"
#include <vector>
#include <memory>

/* CFFile */
vector<shared_ptr<CFFile>> CFFile::ProcessedFile = {};

void CFFile::ProcessFile(const string &FilePath, CFFileType FileType){
    if(CheckIfProcessed(FilePath)){
        // Processed
        return;
    }

    shared_ptr<CFFile> cf = make_shared<CFFile>(FilePath, FileType);
}

bool CFFile::CheckIfProcessed(const string &filepath){
    for (shared_ptr<CFFile> file : ProcessedFile) {
        if(file->FilePath == filepath){
            return true;
        }
    }
    return false;
}

CFFile::CFFile(const string&filepath, CFFileType filetype){
    FilePath = filepath;
    RequestFile(filepath);
}
