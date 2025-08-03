#ifndef SD
#define SD

#include <string>

using namespace std;

enum class AppType{
    Embed, Server, Compiler, Client
};

class Project{
    public:
};

void SetAppType(AppType type);

AppType GetAppType();

void SetProject(const string &projectfile);

#endif
