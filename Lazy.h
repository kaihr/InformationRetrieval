#pragma once
#include "LinkedList.h"

bool calcPos(const wchar_t* inputPath);
bool loadDocFreq(const wchar_t* fileName);
void loadListFromLine(LinkedList* ls, const wchar_t *inputPath, int lineNum);

int getDocFreq(int termID);
void setDocFreq(int termID, int value);

bool saveDocFreq(const wchar_t* fileName);

void initDocFreq();