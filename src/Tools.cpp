#include "Tools.h"

#include <algorithm>

bool StringVecContains(const vector<string> &list, const string &item) {
  if (find(list.begin(), list.end(), item) != list.end()) {
    return true;
  } else {
    return false;
  }
}

bool CharVecContains(const vector<char> &list, const char &item) {
  if (find(list.begin(), list.end(), item) != list.end()) {
    return true;
  } else {
    return false;
  }
}

vector<string> TrimText(const string &Text, const string &Saperator) {
	vector<string> v;
	string s;
	for (char c : Text) {
		bool IsSap = false;
		for (char Sape : Saperator) {
			if(Sape == c){
				IsSap = true;
			}
		}
		if(IsSap){
			if(!s.empty()){
				v.emplace(v.end(), s);
				s.clear();
			}
		}else{
			s += c;
		}
	}

	if(!s.empty()){
		v.emplace(v.end(), s);
		s.clear();
	}
	return v;
}


string GetParentDir(const string &path){
    vector<string> SaperatedFilePath = TrimText(path, "/\\");
    string Directory;
    for(string s: SaperatedFilePath){
        if(s != SaperatedFilePath[SaperatedFilePath.size()-1]){
            Directory = Directory + s + '/';
        }
    }
    return Directory;
}
string GetPathLocationName(const string &path){
    vector<string> SaperatedFilePath = TrimText(path, "/\\");
    return SaperatedFilePath[SaperatedFilePath.size()-1];
}
string GetReletivePath(const string &path, const string &basepath){
    vector<string> SaperatedFilePath = TrimText(path, "/\\");
    vector<string> SaperatedBasePath = TrimText(basepath, "/\\");
    return NULL;
}
