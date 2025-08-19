/*
 * FileData will Load the file and let Reader to read it
 */

#ifndef CODEDATA
#define CODEDATA

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>

using namespace std;

enum class CFFileType{
    None, Template, Project, SourceCode
};

enum class CFCodeType{
    String, Number, Type, Unknown, LangKeyWord
};

class CFCode{
    public:
    int StartPos;
    int EndPos;
    int Line;
    int IntCode;
    CFCodeType CodeType;
    string Content;
};

class CFFile{
    public:
    static void ProcessFile(const string &filepath, CFFileType filetype);
    static vector<shared_ptr<CFFile>> ProcessedFile;
    static bool CheckIfProcessed(const string &filename);
    static shared_ptr<CFFile> FindCFFile(const string &filename);

    stringstream FileContent;
    void Get(const string &filepath, CFFileType filetype);
    void Read();
    CFFileType FileType;
    string FilePath;
    vector<CFCode> Codes;
};

#endif
