#include "FileManager.h"
#include "../SectionData.h"

using namespace std;

void (*RequestFile)(const string &FilePath);

void ReqLocalFile(const string &FilePath){
    RespondFile(FilePath, true);
}

void RespondFile(const string &Content, bool IsPath){
    ReadContent(Content, IsPath);
}

stringstream ReadContent(const string &PathOrContent, bool IsPath){
    stringstream result;
    if(IsPath){
        std::fstream f(PathOrContent);
        if (f.fail()) {
            cout << "\nFailed to open file \"" << PathOrContent <<"\"\n";
        }
        result << f.rdbuf();
    }else{
        result = stringstream(PathOrContent);
    }
    return result;
}
