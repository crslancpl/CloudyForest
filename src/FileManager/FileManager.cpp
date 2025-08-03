#include "FileManager.h"
#include <cstdio>
#include <sstream>
#include <string>

using namespace std;

void (*ReqFileFun)(const char *filepath);

void RequestFile(const char *filepath){
    ReqFileFun(filepath);
}

void SetRequestFileFunc(void (*requestFunc)(const char*)){
    printf("req\n");
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
    ReadContent(content, ispath);
}

stringstream ReadContent(const string &pathorcontent, bool ispath){
    stringstream result;
    if(ispath){
        std::fstream f(pathorcontent);
        if (f.fail()) {
            cout << "\nFailed to open file \"" << pathorcontent <<"\"\n";
        }
        result << f.rdbuf();
    }else{
        result = stringstream(pathorcontent);
    }
    return result;
}
