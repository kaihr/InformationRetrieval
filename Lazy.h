#pragma once
#include "LinkedList.h"

void calcPos(const wchar_t* inputPath);
void loadListFromLine(LinkedList* ls, const wchar_t *inputPath, int lineNum);
void loadAll();