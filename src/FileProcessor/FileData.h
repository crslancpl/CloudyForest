/*
 * FileData will Load the file and let tokenizer to read it
 */

#ifndef CODEDATA
#define CODEDATA

#include "Codes.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>

using namespace std;

enum class CFFileType{
    None, Template, Project, SourceCode
};


class CFCode{
    public:
    int StartPos;
    int EndPos;
    int Line;
    int IntCode;
    BasicCodeTypes CodeType;
    string Content;
};

class CFFile{
    public:
    stringstream FileContent;
    void Get(const string &filepath, CFFileType filetype);
    void Read();
    CFFileType FileType;
    string FilePath;
    string Language;
    LangTemp *LanguageTemplate;
    vector<CFCode> Codes;
};

void ClearProcessedFile();
void ProcessFile(const string &filepath, const string& lang,CFFileType filetype);

bool CheckIfProcessed(const string &filename);
shared_ptr<CFFile> FindCFFile(const string &filename);

#endif
