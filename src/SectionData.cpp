#include "SectionData.h"

AppType Type;

void SetAppType(AppType tyoe){
    Type = tyoe;
}

AppType GetAppType(){
    return Type;
}

void ReadProject(const string &projectfile);
