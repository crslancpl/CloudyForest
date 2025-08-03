/*
 * FileData will Load the file and let Reader to read it
 */

#ifndef CODEDATA
#define CODEDATA

#include <string>
#include <vector>
#include <map>
#include <memory>

using namespace std;

enum class CFFileType{
    Project, SourceCode
};

class CFFile{
    public:
    static map<string,CFFileType> PendingFiles;
    static void ProcessFile(const string &filepath, CFFileType filetype);
    static vector<shared_ptr<CFFile>> ProcessedFile;
    static bool CheckIfProcessed(const string &filename);

    void Get(const string &filepath, CFFileType filetype);
    string FilePath;
    vector<string> Codes;
};

#endif
