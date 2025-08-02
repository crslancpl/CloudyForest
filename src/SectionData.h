#ifndef SD
#define SD

#include <string>

using namespace std;

enum class AppType{
    Embed, Server, Compiler, Client
};

void SetAppType(AppType Tyoe);

AppType GetAppType();

void ReadProject(const string &ProjectFile);

#endif
