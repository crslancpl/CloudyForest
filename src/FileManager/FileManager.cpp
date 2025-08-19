#include "FileManager.h"

#include "../FileProcessor/Tokenizer.h"

#include <cstdio>
#include <memory>
#include <sstream>
#include <string>

using namespace std;

void (*ReqFileFun)(const char *filepath);

void RequestFile(const char *filepath){
    printf("requesting %s\n", filepath);
    ReqFileFun(filepath);
}

void SetRequestFileFunc(void (*requestFunc)(const char*)){
    if (requestFunc == NULL) {
        ReqFileFun = ReqLocalFile;
    }else{
        ReqFileFun = requestFunc;
    }
}

void ReqLocalFile(const char *filepath){
    RespondFile(filepath, filepath, true);
}

void RespondFile(const string &filepath,const string &content, bool ispath){
    printf("responded %s\n", filepath.c_str());
    shared_ptr<CFFile> f = CFFile::FindCFFile(filepath);
    ReadContent( f, content, ispath);
    f->Read();

    // Contents will be stored in CFFile. Reader is no longer needed
}

void ReadContent(shared_ptr<CFFile> f,const string &pathorcontent, bool ispath){
    if(ispath){
        std::fstream fs(pathorcontent);
        if (fs.fail()) {
            cout << "\nFailed to open file \"" << pathorcontent <<"\"\n";
        }
        f->FileContent << fs.rdbuf();
    }else{
        f->FileContent = stringstream(pathorcontent);
    }
}
