#pragma once
#include <stdio.h>

typedef bool (*BrowseFunc)(int& nFiles, const wchar_t* filePath, const wchar_t* pathPath, wchar_t**& pathList, wchar_t** stopWords, int nStopWords);

int isFirstRun();
bool init();

int wchHash(const wchar_t *wstr);

void hashTableInsert(const wchar_t *ch, int docId);
void hashTableRemove(const wchar_t* ch, int docId);

bool listDirectoryContent(const wchar_t* sDir, wchar_t **stopWords, int nStopWords, int &nFilesRead);
bool listDirectoryContent(const wchar_t* path, int& nFiles, const wchar_t* pathPath, wchar_t**& pathList, wchar_t **stopWords, int nStopWords, BrowseFunc f);

void buildHashTable(const wchar_t *path);

bool saveInvIndex(const wchar_t *outputPath);

bool loadInvIndex(const wchar_t* inputPath);

void releaseInvTable();

bool wchComp(void* a, void* b);

void find100Best(wchar_t** pathList, int nFiles, const wchar_t* keywords);

bool addFile(int& nFiles, const wchar_t* filePath, const wchar_t* pathPath, wchar_t**& pathList, wchar_t** stopWords, int nStopWords);
bool remFile(int &nFiles, const wchar_t* filePath, const wchar_t *pathPath, wchar_t**& pathList, wchar_t** stopWords, int nStopWords);

bool isDirectory(const wchar_t *path);

bool addFiles(int &nFiles, const wchar_t* filePath, const wchar_t *pathPath, wchar_t**& pathList, wchar_t** stopWords, int nStopWords);
bool remFiles(int &nFiles, const wchar_t *filePath, const wchar_t *pathPath, wchar_t**& pathList, wchar_t** stopWords, int nStopWords);

void extractResults(FILE* fout, const wchar_t* filePath, const wchar_t* metaDataPath, wchar_t** keyword, int nWords);