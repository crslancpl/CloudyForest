#include "CFBackend.h"
#include "FileProcessor/FileData.h"
#include "SectionData.h"
#include "FileManager/FileManager.h"

#include <iostream>
#include <vector>

using namespace std;

void StartAsServer(){
    //
    SetAppType(AppType::Server);
    cout << "Enter the port to listen: ";
    int Port;
    cin >> Port;
    //ListenToPort();
}

void StartAsClient(){
    //
    SetAppType(AppType::Client);
    cout<<"Enter the url to listen: ";
    string url;
    cin >> url;
    //TellServer();
}

void CompileProject(){
    SetAppType(AppType::Compiler);
    SetRequestFileFunc(NULL);
    cout<<"Enter the path of CFProject: ";
    string Path;
    cin >> Path;
    ProcessFile(Path,"", CFFileType::Project);
}

void QuickCompile(const string &filepath){
    SetAppType(AppType::Compiler);
    SetRequestFileFunc(nullptr);
    ProcessFile(filepath, "project", CFFileType::Project);
}

void QuickCompile(const string &filepath, const string& lang){
    SetAppType(AppType::Compiler);
    SetRequestFileFunc(nullptr);
    LoadLangTemplate(lang);
}


void LoadLangTemplate(const string &lang){
    ProcessFile("syntax/"+ lang + ".txt", lang, CFFileType::Template);
}


void Reload(){
    ClearProcessedFile();
}
