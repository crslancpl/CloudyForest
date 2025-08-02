#ifndef FILEMANAGER
#define FILEMANAGER

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;



void RespondFile(const string &Content, bool IsPath);

stringstream ReadContent(const string &PathOrContent, bool IsPath);

#endif
