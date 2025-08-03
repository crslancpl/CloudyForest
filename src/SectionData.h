#ifndef SD
#define SD

#include <string>

using namespace std;

enum class AppType{
    Embed, Server, Compiler, Client
};

void SetAppType(AppType type);

AppType GetAppType();

void ReadProject(const string &projectfile);

#endif
