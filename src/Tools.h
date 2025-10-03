#ifndef TOOLS
#define TOOLS
#pragma once

#include <string>
#include <vector>

using namespace std;

void Output(const string &text);
void SetOutput(bool OnOff);

bool StringVecContains(const vector<string> &list, const string &item);
bool CharVecContains(const vector<char> &list, const char &item);

vector<string> TrimText(const string &Text, const string &Saperator);

bool IsNumberChar(char c);
bool IsAlphabetChar(char c);

bool StartWith(const string &Text, const string &Pattern);
bool EndWith(const string &Text, const string &Pattern);

string GetParentDir(const string &path);
string GetPathLocationName(const string &path);
string GetRelativePath(const string &path, const string &basepath);

#endif
