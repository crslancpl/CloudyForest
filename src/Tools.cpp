#include "Tools.h"

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
