#include "SectionData.h"
#include "FileProcessor/Codes.h"
#include "Tools.h"

AppType Type;
string ProjectDir;

void SetAppType(AppType tyoe){
    Type = tyoe;
}

AppType GetAppType(){
    return Type;
}

void ReadProject(const string &projectfile);

void SetProject(const string &projectfile){
    //
}

void SetDir(const string &Dirpath){
    if(!EndWith(Dirpath, "/") && !Dirpath.empty()){
        ProjectDir = Dirpath + '/';
    }else{
        ProjectDir = Dirpath;
    }
}

string& GetDir(){
    return ProjectDir;
}
