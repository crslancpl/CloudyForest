#ifndef TOOLS
#define TOOLS

#include <string>
#include <vector>

using namespace std;

vector<string> TrimText(const string &Text, const string &Saperator);

string GetParentDir(const string &path);
string GetPathLocationName(const string &path);
string GetReletivePath(const string &path, const string &basepath);

#endif
