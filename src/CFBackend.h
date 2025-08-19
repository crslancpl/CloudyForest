#ifndef BACKEND
#define BACKEND

#include <string>

using namespace std;

void StartAsServer();
void StartAsClient();
void CompileProject();
void QuickCompile(const string &filepath);
void QuickCompile(const string &filepath, const string& lang);

void LoadLangTemplate(const string &lang);
void Reload();

#endif
