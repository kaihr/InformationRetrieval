#include "TextProcessing.h"
#include "TextNormalizer.h"
#include "stdio.h"
#include "wchar.h"

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
