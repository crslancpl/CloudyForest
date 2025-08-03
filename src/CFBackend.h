#ifndef BACKEND
#define BACKEND

#include <string>

using namespace std;

void StartAsServer();
void StartAsClient();
void CompileProject();
void QuickCompile(const string &filepath);
#endif
