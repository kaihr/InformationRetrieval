#include "Updater.h"
#include "TextProcessing.h"
#include "SimpleSearchEngine.h"

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

bool editFile(int docID, const wchar_t *pathPath, const wchar_t* inputPath, const wchar_t* metaDataPath, wchar_t** stopWordsDic, const int nStopWords)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");
	if (!fin)
		return false;

	FILE* fout = _wfopen(metaDataPath, L"a,ccs=UTF-8");

	wchar_t *text = readFile(inputPath);
	int nSentences = 0;
	wchar_t **sentences = sentenceToken(text, nSentences);

	int wordsAdded = 0;
	for (int i = 0; i < nSentences; i++) {
		int nWords = 0;
		wchar_t** words = wordToken(sentences[i], nWords);

		for (int j = 0; j < nWords; j++) {
			int nextJ = -1;
			//nextJ != -1 --> nextJ is the largest number such that [j, nextJ) is a stop word

			wchar_t currentWord[512] = L"";
			for (int k = j; k < nWords && k < j + 6; k++) {
				if (j != k)
					wcscat(currentWord, L" ");
				wcscat(currentWord, words[k]);

				if (isStopWord(currentWord, stopWordsDic, nStopWords))
					nextJ = k;
			}

			if (nextJ == -1) {
				wcscpy(currentWord, L"");
				for (int k = j; k < nWords && k < j + 6; k++) {
					if (j != k)
						wcscat(currentWord, L" ");
					wcscat(currentWord, words[k]);

					hashTableInsert(currentWord, docID);
					wordsAdded++;
				}
			}
			else
				j = nextJ;
		}
		
		for (int j = 0; j < nWords; j++) {
			fwprintf(fout, L"%ls%lc", words[j], L" \n"[j == nWords - 1]);
		}

		for (int j = 0; j < nWords; j++)
			delete[] words[j];
		delete[] words;
		delete[] sentences[i];
	}

	FILE* pathOut = _wfopen(pathPath, L"a,ccs=UTF-8");
	fwprintf(pathOut, L"%ls\n%ls\n", inputPath, metaDataPath);
	

	fclose(fin);
	fclose(fout);
	fclose(pathOut);

	delete[] sentences;
	delete[] text;

	//wprintf(L"Number of words added to inverted index: %d\n", wordsAdded);

	return true;
}
