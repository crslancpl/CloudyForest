#ifndef INTERPRET
#define INTERPRET

#include <memory>
#include <vector>

#include "FileData.h"

void CFProjectInterp(CFFile *F);

void CFTemplateInterp(CFFile *F);

void CFLangInterp(CFFile *F);

void FindTags(CFFile *F);

void FindNewType(CFFile *F);

void FindFunction(CFFile *F);

#endif
