#pragma once
#include "LinkedList.h"

typedef void (*editF)(const wchar_t *word, int docID);

bool isStopWord(const wchar_t* word, wchar_t** stopWordsDic, int nStopWords);
bool editFile(int docID, const wchar_t *pathPath, const wchar_t *inputPath, const wchar_t *metaDataPath, wchar_t** stopWords, int nStopWords, editF updateFunc);