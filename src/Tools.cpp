#include "Tools.h"

#include <algorithm>

bool StringVecContains(const vector<string> &list, const string &item) {
  return find(list.begin(), list.end(), item) != list.end();
}

bool CharVecContains(const vector<char> &list, const char &item) {
  return find(list.begin(), list.end(), item) != list.end();
}

vector<string> TrimText(const string &Text, const string &Separator) {
	vector<string> result;
	string current;
	
	for (char c : Text) {
		if (Separator.find(c) != string::npos) {
			if (!current.empty()) {
				result.push_back(current);
				current.clear();
			}
		} else {
			current += c;
		}
	}

	if (!current.empty()) {
		result.push_back(current);
	}
	return result;
}


string GetParentDir(const string &path){
    vector<string> separatedFilePath = TrimText(path, "/\\");
    if (separatedFilePath.empty()) return "";
    
    string directory;
    for (size_t i = 0; i < separatedFilePath.size() - 1; i++) {
        directory += separatedFilePath[i] + '/';
    }
    return directory;
}
string GetPathLocationName(const string &path){
    vector<string> separatedFilePath = TrimText(path, "/\\");
    if (separatedFilePath.empty()) return "";
    return separatedFilePath.back();
}
string GetRelativePath(const string &path, const string &basepath){
    vector<string> separatedFilePath = TrimText(path, "/\\");
    vector<string> separatedBasePath = TrimText(basepath, "/\\");
    // TODO: Implement relative path calculation
    return "";
}
