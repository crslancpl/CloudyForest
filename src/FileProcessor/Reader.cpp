/*
 * This will trim the content
 */
#include "Reader.h"
#include <istream>
#include <sstream>
#include <string>


bool ReadFile(stringstream &content){
    //
    char c;

    while (content.get(c)) {
        GetCodes(c);
    }
    return true;
}

void GetCodes(const char &c){
    // Transfer the whole file into symbols and words
}
