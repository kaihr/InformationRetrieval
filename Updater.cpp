#include "Updater.h"
#include "TextProcessing.h"
#include "SimpleSearchEngine.h"
#include "TextNormalizer.h"
#include "Constant.h"

#include <stdio.h>
#include <wchar.h>


bool isStopWord(const wchar_t* word, wchar_t** stopWordsDic, int nStopWords)
{
	int lo = 0, hi = nStopWords - 1;

	while (lo <= hi) {
		int mid = (lo + hi) / 2;

		int foo = wcscmp(word, stopWordsDic[mid]);
		if (foo == 0) {
			return true;
		}

		if (foo < 0)
			hi = mid - 1;
		else
			lo = mid + 1;
	}

	return false;
}

bool editFile(int docID, const wchar_t *pathPath, const wchar_t* inputPath, const wchar_t* metaDataPath, wchar_t** stopWordsDic, const int nStopWords, editF updateFunc)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");
	if (!fin)
		return false;

	FILE* fout = _wfopen(metaDataPath, L"w,ccs=UTF-8");

	wchar_t *text = readFile(inputPath);

	if (text) {
		toLowerCase(text);
		int nSentences = 0;
		wchar_t** sentences = sentenceToken(text, nSentences);

		int wordsAdded = 0;
		wchar_t currentWord[512] = L"";
		wchar_t tmp[512] = L"";
		for (int i = 0; i < nSentences; i++) {
			int nWords = 0;
			wchar_t** words = wordToken(sentences[i], nWords);
			for (int j = 0; j < nWords; j++) {
				int nextJ = -1;
				//nextJ != -1 --> nextJ is the largest number such that [j, nextJ) is a stop word

				if (updateFunc == hashTableInsert) { //Skipping stopwords when inserting. When deleting, if that word doesn't exist, we'll traverse the whole list without doing anything.
					//So it won't affect the correctness of the program.
					wcscpy(currentWord, L"");
					for (int k = j; k < nWords && k < j + N_GRAM; k++) {
						if (j != k)
							wcscat(currentWord, L" ");
						wcscat(currentWord, words[k]);

						if (isStopWord(currentWord, stopWordsDic, nStopWords))
							nextJ = k;
					}
				}


				if (nextJ == -1) {
					wcscpy(currentWord, L"");
					wcscpy(tmp, L"");
					for (int k = j; k < nWords && k < j + N_GRAM; k++) {
						if (j != k)
							wcscat(currentWord, L" ");

						wcscpy(tmp, words[k]);
						toLatinLetter(tmp);
						wcscat(currentWord, tmp);

						//wprintf(L"%ls\n", currentWord);
						updateFunc(currentWord, docID);
						wordsAdded++;
					}
				}
				else
					j = nextJ;
			}


			for (int j = 0; j < nWords; j++) {
				toLatinLetter(words[j]);
				fwprintf(fout, L"%ls%lc", words[j], L" \n"[j == nWords - 1]);
			}

			for (int j = 0; j < nWords; j++)
				delete[] words[j];
			delete[] words;
			delete[] sentences[i];
		}

		delete[] sentences;
	}

	FILE* pathOut = _wfopen(pathPath, L"a,ccs=UTF-8");
	fwprintf(pathOut, L"%ls\n%ls\n", inputPath, metaDataPath);
	
	fclose(fin);
	fclose(fout);
	fclose(pathOut);

	delete[] text;

	//wprintf(L"Number of words added to inverted index: %d\n", wordsAdded);

	return true;
}

bool loadPathList(const wchar_t *pathPath, wchar_t** &pathList, int& nFiles)
{
	for (int i = 0; i < nFiles; i++)
		delete[] pathList[i];
	delete[] pathList;

	wchar_t pathDelim[] = L"\xfeff\n";
	int nFilesRead = 0;
	wchar_t* paths = readFile(pathPath);

	if (!paths)
		return false;

	pathList = splitToken(paths, nFilesRead, pathDelim);
	nFiles = nFilesRead / 2;

	delete[] paths;
	return true;
}

bool savePathList(const wchar_t *pathPath, wchar_t** pathList, int nFiles)
{
	FILE* fout = _wfopen(pathPath, L"w,ccs=UTF-8");

	if (!fout)
		return false;

	for (int i = 0; i < nFiles; i++)
		fwprintf(fout, L"%ls\n%ls\n", pathList[2 * i], pathList[2 * i + 1]);
	fclose(fout);

	return true;
}
