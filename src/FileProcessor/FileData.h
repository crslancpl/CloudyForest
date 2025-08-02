#ifndef CODEDATA
#define CODEDATA

#include <string>
#include <vector>
#include <memory>

using namespace std;

enum class CFFileType{
    Project, SourceCode
};

class CFFile{
    public:
    static void ProcessFile(const string &filepath, CFFileType filetype);
    static vector<shared_ptr<CFFile>> ProcessedFile;
    static bool CheckIfProcessed(const string &filename);

    CFFile(const string&Fflepath, CFFileType filetype);
    string FilePath;
};

#endif
