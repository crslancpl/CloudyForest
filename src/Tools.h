#ifndef TOOLS
#define TOOLS
#pragma once

#include <string>
#include <vector>

using namespace std;

bool StringVecContains(const vector<string> &list, const string &item);
bool CharVecContains(const vector<char> &list, const char &item);


vector<string> TrimText(const string &Text, const string &Saperator);

string GetParentDir(const string &path);
string GetPathLocationName(const string &path);
string GetRelativePath(const string &path, const string &basepath);

#endif
