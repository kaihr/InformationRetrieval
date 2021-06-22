#include "SimpleSearchEngine.h"
#include "TextProcessing.h"
#include "LinkedList.h"
#include "wchar.h"
#include "windows.h"
#include <stdio.h>

const int BUCKET_SIZE = (int)1e6;
FILE* pathFile;
int nFilesRead = 0;

LinkedList *hashTable[BUCKET_SIZE];

bool init()
{
	nFilesRead = 0;
	pathFile = _wfopen(L"path.txt", L"w,ccs=UTF-8");

	if (!pathFile)
		return false;

	for (int i = 0; i < BUCKET_SIZE; i++) {
		hashTable[i] = linkedListInit();

		if (!hashTable[i])
			return false;
	}

	return true;
}

int wchHash(const wchar_t* wstr)
{
	int ans = 0;
	int n = wcslen(wstr);

	for (int i = 0; i < n; i++) {
		long long foo = ((long long)ans) * 257;
		ans = ((foo % BUCKET_SIZE) + wstr[i]) % BUCKET_SIZE;
	}
	
	return ans;
}

void retardSplit(int docId, const wchar_t* inputPath)
{
	wchar_t* text = readFile(inputPath);
	
	wchar_t* currentWord;
	wchar_t* pt;

	wchar_t delim[] = L".,:;'\"!()\n ";

	currentWord = wcstok(text, delim, &pt);

	while (currentWord != nullptr) {
		hashTableInsert(currentWord, docId);
		currentWord = wcstok(nullptr, delim, &pt);
	}

	delete[] text;
}

void hashTableInsert(const wchar_t* ch, int docId)
{
	int toBucket = wchHash(ch);
	bool found = false;
	for (Node* iter = hashTable[toBucket]->pHead; iter; iter = iter->nxt) {
		if (iter->value == docId) {
			found = true;
			break;
		}
	}

	if (!found)
		insertBack(hashTable[toBucket], docId);
}

bool listDirectoryContent(const wchar_t* sDir)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    wchar_t sPath[2048];

    wsprintf(sPath, L"%s\\*.*", sDir);

    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
        return false;

    do
    {
        if (wcscmp(fdFile.cFileName, L".") != 0 && wcscmp(fdFile.cFileName, L"..") != 0)
        {
            swprintf(sPath, L"%s\\%s", sDir, fdFile.cFileName);
            if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                wprintf(L"Directory: %s\n", sPath);
                listDirectoryContent(sPath);
            }
            else {
				fwprintf(pathFile, L"%ls\n", sPath);
				retardSplit(nFilesRead, sPath);
				nFilesRead++;
            }
        }
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);
    return true;
}


void buildHashTable(const wchar_t* path)
{
	if (!init()) {
		fwprintf(stderr, L"Not enough memory\n");
		return;
	}

	listDirectoryContent(path);

	if (!saveInvTable(L"inverted_index.txt"))
		fwprintf(stderr, L"Can not save inverted index table\n");

	fclose(pathFile);
}

bool saveInvTable(const wchar_t* outputPath)
{
	FILE* fout = _wfopen(outputPath, L"w,ccs=UTF-8");

	if (!fout)
		return false;

	for (int i = 0; i < BUCKET_SIZE; i++) {
		int len = 0;
		for (Node* iter = hashTable[i]->pHead; iter; iter = iter->nxt)
			len++;

		fwprintf(fout, L"%d ", len);

		for (Node* iter = hashTable[i]->pHead; iter; iter = iter->nxt)
			fwprintf(fout, L"%d ", iter->value);

		fwprintf(fout, L"\n");

		eraseLinkedList(hashTable[i]);
	}

	return true;
}

bool loadInvTable(const wchar_t* outputPath)
{
	FILE* fin = _wfopen(outputPath, L"r,ccs=UTF-8");

	if (!fin)
		return false;

	for (int i = 0; i < BUCKET_SIZE; i++) {
		hashTable[i] = linkedListInit();
		if (!hashTable[i])
			return false;
	}

	for (int i = 0; i < BUCKET_SIZE; i++) {
		if (i % (int)(BUCKET_SIZE / 10) == 0) {
			fprintf(stderr, "Loading...\n");
		}
		int size = 0;
		fwscanf(fin, L"%d", &size);
		while (size-- > 0) {
			int x = 0;
			fwscanf(fin, L"%d", &x);
			insertBack(hashTable[i], x);
		}
	}

	fclose(fin);

	return true;
}
