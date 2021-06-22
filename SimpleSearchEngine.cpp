#include "SimpleSearchEngine.h"
#include "TextProcessing.h"
#include "TextNormalizer.h"
#include "LinkedList.h"
#include "Sort.h"
#include "Updater.h"
#include "Pair.h"

#include <stdio.h>
#include <wchar.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

const int BUCKET_SIZE = (int)1e6;

int nFilesRead = 0;
LinkedList *hashTable[BUCKET_SIZE];

bool init()
{
	nFilesRead = 0;

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

bool listDirectoryContent(const wchar_t* sDir, wchar_t **stopWords, int nStopWords)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = nullptr;

    wchar_t sPath[2048] = L"";

	wcscat(sPath, sDir);
    wcscat(sPath, L"\\*.*");

	hFind = FindFirstFile(sPath, &fdFile);

	if (hFind == INVALID_HANDLE_VALUE)
		return false;

    do
    {
		if (wcscmp(fdFile.cFileName, L".") == 0 || wcscmp(fdFile.cFileName, L"..") == 0)
			continue;

		wcscpy(sPath, sDir);
		wcscat(sPath, L"\\");
		wcscat(sPath, fdFile.cFileName);

		if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			wprintf(L"Directory: %ls\n", sPath);
			listDirectoryContent(sPath, stopWords, nStopWords);
		}
		else {
			wchar_t metaDataPath[2048] = L"";
			wcscpy(metaDataPath, L"compressed\\");
			wchar_t outputName[20];

			for (int i = 0; i < 10; i++)
				outputName[i] = (wchar_t)((rand() % 26) + 'a');
			outputName[10] = L'\0';

			wcscat(metaDataPath, outputName);
			wcscat(metaDataPath, L".txt");

			editFile(nFilesRead, L"path.txt", sPath, metaDataPath, stopWords, nStopWords);
			nFilesRead++;
		}
	} while (FindNextFile(hFind, &fdFile));
	
	FindClose(hFind);
	return true;
}

bool wchComp(void* a, void* b) {
	wchar_t* lhs = *((wchar_t**)a);
	wchar_t* rhs = *((wchar_t**)b);

	if (wcscmp(lhs, rhs) < 0)
		return true;

	return false;
}

void find100Best(wchar_t **pathList, int nFiles, const wchar_t* originalKeywords)
{
	Pair *score = new Pair[nFiles];

	for (int i = 0; i < nFiles; i++) {
		score[i].a = 0;
		score[i].b = i;
	}

	int nTok = 0;

	wchar_t modifiedKeyWords[256];
	wcscpy(modifiedKeyWords, originalKeywords);

	toLatinLetter(modifiedKeyWords);
	wchar_t** token = splitToken(modifiedKeyWords, nTok, L" .,:;'\"!()\n");

	wchar_t currentWord[512];

	for (int i = 0; i < nTok; i++) {
		wcscpy(currentWord, L"");
		for (int j = i; j < nTok && j < i + 6; j++) {
			if (j != i)
				wcscat(currentWord, L" ");
			wcscat(currentWord, token[j]);

			//wprintf(L"%ls\n", currentWord);
			int index = wchHash(currentWord);
			for (Node* iter = hashTable[index]->pHead; iter; iter = iter->nxt) {
				score[iter->value].a += (1 << (j - i + 1));
			}
		}
	}

	mergeSort((void*)score, nFiles, sizeof(Pair), pairCmp);

	wprintf(L"Search results for: %ls", originalKeywords);

	for (int i = 0; i < 100; i++) {
		int currentScore = score[i].a;

		if (currentScore == 0)
			break;

		int currentFileId = score[i].b;

		wprintf(L"- %ls, rating: %d\n", pathList[currentFileId * 2], currentScore);

		if ((i + 1) % 10 == 0) {
			wprintf(L"Continue searching ? (Enter 1 to continue, other keys to stop): ");
			int option;
			wscanf(L"%d", &option);
			if (option != 1)
				break;
		}
	}

	getchar();
	wprintf(L"Press any button to continue\n");
	getchar();

	delete[] score;
}

void buildHashTable(const wchar_t* path)
{
	time_t beginTime = clock();
	if (!init()) {
		fwprintf(stderr, L"Not enough memory\n");
		return;
	}

	FILE* dummy = _wfopen(L"path.txt", L"w,ccs=UTF-8");
	fclose(dummy);

	int nWords = 0;
	wchar_t* text = readFile(L"resources\\vietnamese_stopwords.txt");
	wchar_t** stopWords = sentenceToken(text, nWords);

	if (!stopWords) {
		fwprintf(stderr, L"Can not load stopwords.\n");
		return;
	}

	mergeSort((void*)stopWords, nWords, sizeof(wchar_t*), wchComp);

	listDirectoryContent(path, stopWords, nWords);

	if (!saveInvTable(L"inverted_index.txt"))
		fwprintf(stderr, L"Can not save inverted index table\n");

	for (int i = 0; i < nWords; i++)
		delete[] stopWords[i];
	delete[] stopWords;
	delete[] text;

	fprintf(stderr, "Elapsed time: %.5lf seconds", (clock() - beginTime) * 1.0 / CLOCKS_PER_SEC);
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

int loadInvTable(const wchar_t* inputPath)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");

	if (!fin)
		return -1;

	for (int i = 0; i < BUCKET_SIZE; i++) {
		hashTable[i] = linkedListInit();
		if (!hashTable[i])
			return -1;
	}

	nFilesRead = 0;

	for (int i = 0; i < BUCKET_SIZE; i++) {
		if (i % (int)(BUCKET_SIZE / 10) == 0) {
			fprintf(stderr, "Loading...\n");
		}
		int size = 0;
		fwscanf(fin, L"%d", &size);
		while (size-- > 0) {
			int x = 0;
			fwscanf(fin, L"%d", &x);

			if (x + 1 > nFilesRead)
				nFilesRead = x + 1;

			insertBack(hashTable[i], x);
		}
	}

	fclose(fin);

	return nFilesRead;
}

void releaseInvTable()
{
	for (int i = 0; i < BUCKET_SIZE; i++) {
		if (i % ((int)BUCKET_SIZE / 10) == 0)
			fprintf(stderr, "Unloading...\n");
		eraseLinkedList(hashTable[i]);
	}
}
