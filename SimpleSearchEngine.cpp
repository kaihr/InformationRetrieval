#include "SimpleSearchEngine.h"
#include "TextProcessing.h"
#include "TextNormalizer.h"
#include "LinkedList.h"
#include "Sort.h"
#include "Updater.h"
#include "Pair.h"
#include "FastInputOutput.h"
#include "Constant.h"
#include "Lazy.h"

#include <stdio.h>
#include <wchar.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

int nFilesRead = 0;
LinkedList *hashTable[BUCKET_SIZE];

int isFirstRun()
{
	FILE* fin = _wfopen(L"config.txt", L"r,ccs=UTF-8");

	if (!fin)
		return 1;

	int foo;
	fwscanf(fin, L"%d", &foo);

	fclose(fin);
	return foo;
}

bool init()
{
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

	//wait...
	//how about you just check the last element because the docID are insert in ascending order anyway

	loadListFromLine(hashTable[toBucket], L"inverted_index.txt", toBucket);
	if (!isEmpty(hashTable[toBucket]) && (hashTable[toBucket]->pTail->value == docId))
		found = true;

	/*for (Node* iter = hashTable[toBucket]->pHead; iter; iter = iter->nxt) {
		if (iter->value == docId) {
			found = true;
			break;
		}
	}*/

	if (!found)
		insertBack(hashTable[toBucket], docId);
}

void hashTableRemove(const wchar_t* ch, int docId)
{
	int toBucket = wchHash(ch);
	loadListFromLine(hashTable[toBucket], L"inverted_index.txt", toBucket);
	eraseValue(hashTable[toBucket], docId);
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

			editFile(nFilesRead, L"path.txt", sPath, metaDataPath, stopWords, nStopWords, hashTableInsert);
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
			loadListFromLine(hashTable[index], L"inverted_index.txt", index);
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

bool addFile(int& nFiles, const wchar_t* filePath, const wchar_t* pathPath, wchar_t**& pathList, wchar_t **stopWords, int nStopWords)
{
	wchar_t metaDataPath[2048] = L"";
	wcscpy(metaDataPath, L"compressed\\");
	wchar_t outputName[20];

	for (int i = 0; i < 10; i++)
		outputName[i] = (wchar_t)((rand() % 26) + 'a');
	outputName[10] = L'\0';

	wcscat(metaDataPath, outputName);
	wcscat(metaDataPath, L".txt");

	if (!editFile(nFiles, pathPath, filePath, metaDataPath, stopWords, nStopWords, hashTableInsert))
		return false;

	for (int i = 0; i < nFiles; i++)
		delete[] pathList[i];
	delete[] pathList;

	nFiles++;

	wchar_t pathDelim[] = L"\xfeff\n";
	int nFilesRead = 0;
	wchar_t* paths = readFile(L"path.txt");
	pathList = splitToken(paths, nFilesRead, pathDelim);

	delete[] paths;

	assert(nFilesRead / 2 == nFiles);

	return true;
}

bool remFile(int nFiles, const wchar_t* filePath, const wchar_t* pathPath, wchar_t** pathList, wchar_t** stopWords, int nStopWord)
{
	int docId = -1;
	for (int i = 0; i < nFiles; i++) {
		if (wcscmp(filePath, pathList[2 * i]) == 0) {
			docId = i;
			break;
		}
	}

	if (docId == -1)
		return false;

	editFile(docId, pathPath, pathList[2 * docId + 1], L"eHtIcSjTqOnRmYtgs", stopWords, nStopWord, hashTableRemove);

	wcscpy(pathList[2 * docId], L"etcjqnmtgs");
	wcscpy(pathList[2 * docId + 1], L"etcjqnmtgs");

	FILE* fout = _wfopen(pathPath, L"w,ccs=UTF-8");
	for (int i = 0; i < nFiles; i++)
		fwprintf(fout, L"%ls\n%ls\n", pathList[2 * i], pathList[2 * i + 1]);
	fclose(fout);

	return true;
}

void buildHashTable(const wchar_t* path)
{
	loadAll();
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
	delete[] text;

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
	releaseInvTable();

	FILE* config = _wfopen(L"config.txt", L"w,ccs=UTF-8");
	fwprintf(config, L"0\n");
	fclose(config);

	fprintf(stderr, "Elapsed time: %.5lf seconds", (clock() - beginTime) * 1.0 / CLOCKS_PER_SEC);
}

bool saveInvTable(const wchar_t* outputPath)
{
	loadInvTable(L"inverted_index.txt");

	FILE* fout = _wfopen(outputPath, L"w,ccs=UTF-8");

	if (!fout)
		return false;

	for (int i = 0; i < BUCKET_SIZE; i++) {
		int len = 0;

		for (Node* iter = hashTable[i]->pHead; iter; iter = iter->nxt)
			len++;

		writeInt(fout, len);
		fputwc(L' ', fout);

		for (Node* iter = hashTable[i]->pHead; iter; iter = iter->nxt) {
			writeInt(fout, iter->value);
			fputwc(L' ', fout);
		}

		fputwc(L'\n', fout);
	}

	fclose(fout);

	return true;
}

bool loadInvTable(const wchar_t* inputPath)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");

	if (!fin)
		return false;

	for (int i = 0; i < BUCKET_SIZE; i++) {
		loadListFromLine(hashTable[i], L"inverted_index.txt", i);		
		int size = 0;
		size = readInt(fin);
		while (size-- > 0) {
			int x = 0;
			x = readInt(fin);
			insertBack(hashTable[i], x);
		}
	}

	fclose(fin);
	return true;
}

void releaseInvTable()
{
	for (int i = 0; i < BUCKET_SIZE; i++) {
		if (i % ((int)BUCKET_SIZE / 10) == 0)
			fprintf(stderr, "Unloading...\n");
		eraseLinkedList(hashTable[i]);
	}
}
