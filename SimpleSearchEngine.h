#pragma once

bool init();

int wchHash(const wchar_t *wstr);
void retardSplit(int docId, const wchar_t* inputPath);

void hashTableInsert(const wchar_t *ch, int docId);

bool listDirectoryContent(const wchar_t* sDir);

void buildHashTable(const wchar_t *path);

bool saveInvTable(const wchar_t *outputPath);

bool loadInvTable(const wchar_t* outputPath);
