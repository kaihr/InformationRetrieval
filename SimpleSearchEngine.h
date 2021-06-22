#pragma once

bool init();

int wchHash(const wchar_t *wstr);

void hashTableInsert(const wchar_t *ch, int docId);

bool listDirectoryContent(const wchar_t* sDir, wchar_t **stopWords, int nStopWords);

void buildHashTable(const wchar_t *path);

bool saveInvTable(const wchar_t *outputPath);

int loadInvTable(const wchar_t* inputPath);

void releaseInvTable();
