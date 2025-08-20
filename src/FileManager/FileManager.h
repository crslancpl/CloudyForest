/*
 * FileManager will Get the file for FileData
 */

#ifndef FILEMANAGER
#define FILEMANAGER

#include "../FileProcessor/FileData.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>

using namespace std;

void RequestFile(const char *filepath);

void SetRequestFileFunc(void (*requestFunc)(const char*));

void ReqLocalFile(const char *filepath);

void RespondFile(const string &filepath,const string &content, bool ispath);

void ReadContent(shared_ptr<CFFile>& f,const string &pathorcontent, bool ispath);

#endif
