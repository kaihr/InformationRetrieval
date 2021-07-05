#include "TextProcessing.h"
#include "TextNormalizer.h"
#include "Constant.h"
#include "SimpleSearchEngine.h"
#include "Sort.h"
#include "Lazy.h"

#include <stdio.h>
#include <wchar.h>
#include <math.h>

wchar_t* readFile(const wchar_t* inputPath)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");

	if (!fin)
		return nullptr;

	int fileLen = 0;
	while (true) {
		wchar_t ch = fgetwc(fin);

		if (feof(fin))
			break;

		fileLen++;
	}

	if (fileLen == 0)
		return nullptr;

	wchar_t* wstr = new wchar_t[fileLen + 3];
	int iter = 0;

	fseek(fin, 0, SEEK_SET);
	
	while (true) {
		wchar_t ch = fgetwc(fin);

		if(feof(fin))
			break;

		wstr[iter++] = ch;
	}

	wstr[iter++] = L'\0';

	fclose(fin);

	return wstr;
}

wchar_t** splitToken(const wchar_t* _text, int& nTok, const wchar_t *delim)
{
	int length = wcslen(_text);
	wchar_t* text = new wchar_t[length + 1];
	wchar_t* currentTok;
	wchar_t* pt = nullptr;
	wcscpy(text, _text);

	nTok = 0;
	currentTok = wcstok(text, delim, &pt);
	while (currentTok != nullptr) {
		nTok++;

		currentTok = wcstok(nullptr, delim, &pt);
	}

	wchar_t** sentences = new wchar_t* [nTok];
	wcscpy(text, _text);

	nTok = 0;
	currentTok = wcstok(text, delim, &pt);
	while (currentTok != nullptr) {
		sentences[nTok] = new wchar_t[wcslen(currentTok) + 1];
		wcscpy(sentences[nTok], currentTok);
		nTok++;

		currentTok = wcstok(nullptr, delim, &pt);
	}

	delete[] text;
	return sentences;
}

wchar_t** wordToken(const wchar_t* _text, int& nWords)
{
	return splitToken(_text, nWords, L" ");
}

wchar_t** sentenceToken(const wchar_t* _text, int &nSentences)
{
	return splitToken(_text, nSentences, L".,:;'\"!()\n");
}

double getDocNorm(const wchar_t* inputPath, int nFiles)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");

	if (!fin)
		return 0.0;

	double ans = 0.0;

	wchar_t* text = readFile(inputPath);

	int nDim = 0;

	int nSentences = 0;

	wchar_t** sentence = sentenceToken(text, nSentences);

	for (int i = 0; i < nSentences; i++) {
		int nWords = 0;
		wchar_t** word = wordToken(sentence[i], nWords);
		
		nDim += nWords;

		for (int i = 0; i < nWords; i++)
			delete[] word[i];
		delete[] word;
	}

	nDim *= N_GRAM;

	int* termsID = new int[nDim];

	nDim = 0;

	wchar_t currentWord[512] = L"";

	for (int i = 0; i < nSentences; i++) {
		int nWords = 0;
		wchar_t** word = wordToken(sentence[i], nWords);

		for (int j = 0; j < nWords; j++) {
			wcscpy(currentWord, L"");
			for (int k = j; k < nWords && k < j + N_GRAM; k++) {
				if (j != k)
					wcscat(currentWord, L" ");
				wcscat(currentWord, word[k]);

				termsID[nDim++] = wchHash(currentWord);
			}
		}

		for (int i = 0; i < nWords; i++)
			delete[] word[i];
		delete[] word;
	}

	mergeSort((void*)termsID, nDim, sizeof(int), intCmp);

	for (int i = 0; i < nDim; ) {
		int j = i;
		while (j < nDim && termsID[i] == termsID[j])
			j++;

		int tf = j - i;
		int df = abs(getDocFreq(termsID[i]));

		double idf = log10(1.0 * nFiles / (1 + df));

		double curWeight = tf * idf;
		
		//fwprintf(stderr, L"%d %.4lf %.4lf\n", tf, idf, curWeight);

		ans += curWeight * curWeight;
		
		i = j;
	}

	for (int i = 0; i < nSentences; i++)
		delete[] sentence[i];
	delete[] sentence;

	fclose(fin);
	delete[] text;

	delete[] termsID;

	return ans;
}
