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
#include "Tuple.h"
#include "StringMatching.h"

#include <stdio.h>
#include <wchar.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <direct.h>

LinkedList *invIndex[NUMBER_OF_TERMS];

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
	for (int i = 0; i < NUMBER_OF_TERMS; i++) {
		invIndex[i] = linkedListInit();

		if (!invIndex[i])
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
		ans = ((foo % NUMBER_OF_TERMS) + wstr[i]) % NUMBER_OF_TERMS;
	}
	
	return ans;
}

void hashTableInsert(const wchar_t* ch, int docId)
{
	int termID = wchHash(ch);
	bool found = false;

	//wait...
	//how about you just check the last element because the docID are insert in ascending order anyway

	loadListFromLine(invIndex[termID], L"inverted_index.txt", termID);

	if (!isEmpty(invIndex[termID]) && (invIndex[termID]->pTail->value == docId))
		found = true;

	if (!found) {
		setDocFreq(termID, getDocFreq(termID) + 1);
		insertBack(invIndex[termID], docId);
	}

	invIndex[termID]->pTail->freq++;
}

void hashTableRemove(const wchar_t* ch, int docId)
{
	int termID = wchHash(ch);
	loadListFromLine(invIndex[termID], L"inverted_index.txt", termID);
	eraseValue(invIndex[termID], docId);
	setDocFreq(termID, getDocFreq(termID) - 1);
}

bool listDirectoryContent(const wchar_t* sDir, wchar_t **stopWords, int nStopWords, int &nFilesRead)
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
			listDirectoryContent(sPath, stopWords, nStopWords, nFilesRead);
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
	modifiedKeyWords[wcslen(modifiedKeyWords) - 1] = '\0';

	wchar_t currentWord[512];

	int* uniqueSet = new int[nTok * N_GRAM];
	int uniqueSetSize = 0;

	for (int i = 0; i < nTok; i++) {
		wcscpy(currentWord, L"");
		for (int j = i; j < nTok && j < i + N_GRAM; j++) {
			if (j != i)
				wcscat(currentWord, L" ");
			wcscat(currentWord, token[j]);
			
			int termID = wchHash(currentWord);
			uniqueSet[uniqueSetSize++] = termID;
		}
	}

	mergeSort((void*)uniqueSet, uniqueSetSize, sizeof(int), intCmp);
	
	for (int i = 0; i < uniqueSetSize; ) {
		int j = i;
		while (j < uniqueSetSize && uniqueSet[i] == uniqueSet[j])
			j++;

		int termID = uniqueSet[i];
		loadListFromLine(invIndex[termID], L"inverted_index.txt", termID);
		
		double df = getDocFreq(termID);

		double querytf = (j - i);
		double queryidf = log10(1.0 * nFiles / (1 + df));
		double queryWeight = querytf * queryidf;

		
		for (Node* iter = invIndex[termID]->pHead; iter; iter = iter->nxt) {
			double doctf = iter->freq;
			double docidf = log10(1.0 * nFiles / (1 + df));
			double docWeight = doctf * docidf;

			score[iter->value].a += queryWeight * docWeight;
		}

		i = j;
	}

	mergeSort((void*)score, nFiles, sizeof(Pair), pairCmp);

	for (int i = 0; i < 120 && i < nFiles; i++)
		score[i].a = score[i].a / sqrt(getDocNorm(pathList[2 * score[i].b + 1], nFiles));

	mergeSort((void*)score, 120, sizeof(Pair), pairCmp);


	for (int i = 0; i < 60 && i < nFiles; ) {
		system("cls");
		wprintf(L"=== Search results for: %ls ===\n", modifiedKeyWords);
		int j = i;
		
		int num = 0;

		while (j < 60 && j < nFiles && j < i + 10 && score[j].a > 0.0) {
			double currentScore = score[j].a;
			int currentFileId = score[j].b;

			wchar_t* absolutePath = _wfullpath(nullptr, pathList[2 * currentFileId], 4096);

			wprintf(L"%2d. %ls\n", ++num, absolutePath);
			delete[] absolutePath;

			j++;
		}

		wprintf(L"=======================================================\n");

		if (num == 0) {
			wprintf(L"Nothing to be found.\n");
			break;
		}

		wprintf(L"- Type a number (1 - %d) to view the correspoding text.\n", num);
		wprintf(L"- Type 0 to stop searching.\n");
		wprintf(L"- Type 11 to show more results.\n");
		int option = 0;
		
		do {
			wprintf(L"Your choice?: ");
			wscanf(L"%d", &option);
		} while (!(option == 0 || option == 11 || (1 <= option && option <= num)));

		if (option == 0)
			break;

		if (option == 11) {
			i = j;
			continue;
		}

		option--;

		int docID = score[i + option].b;

		wchar_t *absolutePath = _wfullpath(nullptr, pathList[2 * docID], 4096);

		system("cls");
		extractResults(stdout, absolutePath, pathList[2 * docID + 1], token, nTok);

		delete[] absolutePath;
		
		setbuf(stdin, nullptr);
		wprintf(L"Press Enter to continue\n");
		getchar();
	}

	setbuf(stdin, nullptr);
	wprintf(L"Press Enter to continue\n");
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
	for (int i = 0; (i < nFiles) && (docId == -1); i++) {
		wchar_t *fileAbsolutePath;
		fileAbsolutePath = _wfullpath(nullptr, filePath, 4096);

		wchar_t* listAbsolutePath;
		listAbsolutePath = _wfullpath(nullptr, pathList[2 * i], 4096);

		if (wcscmp(fileAbsolutePath, listAbsolutePath) == 0)
			docId = i;
		
		delete[] fileAbsolutePath;
		delete[] listAbsolutePath;
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

void extractResults(FILE* fout, const wchar_t* filePath, const wchar_t* metaDataPath, wchar_t** keyword, int nWords)
{
	//extract at most 5 sentences contain information relate to keyword
	fwprintf(fout, L"=== Search results for \"");

	for (int i = 0; i < nWords; i++) {
		if (i != 0)
			fwprintf(fout, L" ");
		fwprintf(fout, L"%ls", keyword[i]);
	}

	fwprintf(fout, L"\" in \"%ls\" ===\n", filePath);

	wchar_t* text = readFile(metaDataPath);
	int nSentences = 0;
	wchar_t** sentence = sentenceToken(text, nSentences);
	delete[] text;

	//for (int i = 0; i < nSentences; i++)
	//	fwprintf(stderr, L"%ls\n", sentence[i]);

	int cntMark = 0;

	bool* filter = new bool[nSentences + 1];

	for (int i = 0; i <= nSentences; i++)
		filter[i] = false;

	wchar_t pattern[512];

	for (int length = N_GRAM; length > 0 && cntMark < 5; length--) {
		for (int startPos = 0; startPos + length - 1 < nWords && cntMark < 5; startPos++) {
			wcscpy(pattern, L"");
			for (int i = 0; i < length; i++) {
				if (i != 0)
					wcscat(pattern, L" ");
				wcscat(pattern, keyword[startPos + i]);
			}

			for (int i = 0; i < nSentences && cntMark < 5; i++) {
				if (filter[i])
					continue;

				if (isMatched(sentence[i], pattern)) {
					//fwprintf(stderr, L"%ls matched %ls\n", sentence[i], pattern);
					cntMark++;
					filter[i] = true;
				}
			}
		}
	}

	text = readFile(filePath);
	int orgTextNSentences = 0;
	wchar_t** originalText = sentenceToken(text, orgTextNSentences);
	delete[] text;

	int actualSentenceID = 0;

	for (int i = 0; i < orgTextNSentences; i++)
	{
		int nWordInSentence = 0;

		for (int j = 0, curLen = wcslen(originalText[i]); j < curLen; j++) {
			wchar_t foo = toLatinLetter(originalText[i][j]);
			if (foo == L' ')
				originalText[i][j] = L' ';
		}

		wchar_t** wordTok = wordToken(originalText[i], nWordInSentence);

		if (nWordInSentence) {
			if (filter[actualSentenceID]) {
				fwprintf(fout, L" -- ");
				for (int j = 0; j < nWordInSentence; j++)
					fwprintf(fout, L"%ls%lc", wordTok[j], L" \n"[j == nWordInSentence - 1]);
			}

			actualSentenceID++;
		}

		for (int j = 0; j < nWordInSentence; j++)
			delete[] wordTok[j];
		delete[] wordTok;
	}

	delete[] filter;
	for (int i = 0; i < orgTextNSentences; i++)
		delete[] originalText[i];
	delete[] originalText;
	for (int i = 0; i < nSentences; i++)
		delete[] sentence[i];
	delete[] sentence;
}

void buildHashTable(const wchar_t* path)
{
	initDocFreq();

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

	int nFilesRead = 0;

	_wmkdir(L"compressed");

	listDirectoryContent(path, stopWords, nWords, nFilesRead);

	for (int i = 0; i < nWords; i++)
		delete[] stopWords[i];
	delete[] stopWords;

	if (!saveInvIndex(L"inverted_index.txt"))
		fwprintf(stderr, L"Can not save inverted index table\n");

	if (!saveDocFreq(L"doc_freq.txt"))
		fwprintf(stderr, L"Can not save documents frequencies\n");

	releaseInvTable();

	FILE* config = _wfopen(L"config.txt", L"w,ccs=UTF-8");
	fwprintf(config, L"0\n");
	fclose(config);

	fprintf(stderr, "Elapsed time: %.5lf seconds", (clock() - beginTime) * 1.0 / CLOCKS_PER_SEC);
}

bool saveInvIndex(const wchar_t* outputPath)
{
	loadInvIndex(L"inverted_index.txt");

	FILE* fout = _wfopen(outputPath, L"w,ccs=UTF-8");

	if (!fout)
		return false;

	for (int i = 0; i < NUMBER_OF_TERMS; i++) {
		int len = 0;

		for (Node* iter = invIndex[i]->pHead; iter; iter = iter->nxt)
			len++;

		writeInt(fout, len);
		fputwc(L' ', fout);

		for (Node* iter = invIndex[i]->pHead; iter; iter = iter->nxt) {
			writeInt(fout, iter->value);
			fputwc(L' ', fout);
			writeInt(fout, iter->freq);
			fputwc(L' ', fout);
		}

		fputwc(L'\n', fout);
	}

	fclose(fout);

	return true;
}

bool loadInvIndex(const wchar_t* inputPath)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");

	if (!fin)
		return false;

	for (int i = 0; i < NUMBER_OF_TERMS; i++) {
		if(i % (NUMBER_OF_TERMS / 10) == 0)
			wprintf(L"Loading...\n");

		loadListFromLine(invIndex[i], L"inverted_index.txt", i);
	}

	fclose(fin);
	return true;
}

void releaseInvTable()
{
	for (int i = 0; i < NUMBER_OF_TERMS; i++) {
		if (i % ((int)NUMBER_OF_TERMS / 10) == 0)
			fprintf(stderr, "Unloading...\n");
		eraseLinkedList(invIndex[i]);
	}
}
