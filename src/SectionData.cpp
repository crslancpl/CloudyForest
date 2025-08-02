#include "SectionData.h"

AppType appType;

void SetAppType(AppType Tyoe){
    appType = Tyoe;
}

AppType GetAppType(){
    return appType;
}

void ReadProject(const string &ProjectFile);
