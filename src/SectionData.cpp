#include "SectionData.h"

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
