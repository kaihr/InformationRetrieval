#pragma once
#include <stdio.h>

int isFirstRun();
bool init();

int wchHash(const wchar_t *wstr);

void hashTableInsert(const wchar_t *ch, int docId);
void hashTableRemove(const wchar_t* ch, int docId);

bool listDirectoryContent(const wchar_t* sDir, wchar_t **stopWords, int nStopWords);

void buildHashTable(const wchar_t *path);

bool saveInvIndex(const wchar_t *outputPath);

bool loadInvIndex(const wchar_t* inputPath);

void releaseInvTable();

bool wchComp(void* a, void* b);

void find100Best(wchar_t** pathList, int nFiles, const wchar_t* keywords);

bool addFile(int& nFiles, const wchar_t* filePath, const wchar_t* pathPath, wchar_t**& pathList, wchar_t** stopWords, int nStopWords);
bool remFile(int nFiles, const wchar_t* filePath, const wchar_t *pathPath, wchar_t** pathList, wchar_t** stopWords, int nStopWord);

void extractResults(FILE* fout, const wchar_t* filePath, const wchar_t* metaDataPath, wchar_t** keyword, int nWords);