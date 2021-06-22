#include "TextProcessing.h"
#include "TextNormalizer.h"
#include "stdio.h"
#include "wchar.h"

wchar_t* readFile(const wchar_t* inputPath)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");

	if (!fin)
		return nullptr;

	wchar_t last = 65535;
	int fileLen = 0;
	while (true) {
		wchar_t ch = fgetwc(fin);

		if (feof(fin))
			break;

		ch = wideCharNormalize(ch);

		if (ch == ' ' || ch == '\n') {
			if (ch == last || last == 65535)
				continue;
		}

		last = ch;
		fileLen++;
	}

	if (fileLen == 0)
		return nullptr;

	wchar_t* wstr = new wchar_t[fileLen + 3];
	int iter = 0;

	last = 65535;
	fclose(fin);

	fin = _wfopen(inputPath, L"r,ccs=UTF-8");

	while (true) {
		wchar_t ch = fgetwc(fin);
		ch = wideCharNormalize(ch);

		if (feof(fin))
			break;

		if (ch == ' ' || ch == '\n') {
			if (ch == last || last == 65535)
				continue;
		}

		wstr[iter++] = ch;
		last = ch;
	}

	wstr[iter++] = L'\0';

	fclose(fin);

	return wstr;
}

wchar_t** wordToken(const wchar_t* _text, int &nWords)
{
	int length = wcslen(_text);
	wchar_t* text = new wchar_t[length + 1];
	wchar_t* currentWord;
	wchar_t* pt = nullptr;
	wchar_t delim[] = L" ";
	wcscpy(text, _text);

	nWords = 0;
	currentWord = wcstok(text, delim, &pt);
	while (currentWord != nullptr) {
		nWords++;

		currentWord = wcstok(nullptr, delim, &pt);
	}

	wchar_t** words = new wchar_t* [nWords];
	wcscpy(text, _text);
	
	nWords = 0;
	currentWord = wcstok(text, delim, &pt);
	while (currentWord != nullptr) {
		words[nWords] = new wchar_t[wcslen(currentWord) + 1];
		wcscpy(words[nWords], currentWord);
		nWords++;

		currentWord = wcstok(nullptr, delim, &pt);
	}

	delete[] text;
	return words;
}

wchar_t** sentenceToken(const wchar_t* _text, int &nSentences)
{
	int length = wcslen(_text);
	wchar_t* text = new wchar_t[length + 1];
	wchar_t* currentSentence;
	wchar_t* pt = nullptr;
	wchar_t delim[] = L".,:;'\"!()\n";
	wcscpy(text, _text);

	nSentences = 0;
	currentSentence = wcstok(text, delim, &pt);
	while (currentSentence != nullptr) {
		nSentences++;

		currentSentence = wcstok(nullptr, delim, &pt);
	}

	wchar_t** sentences = new wchar_t* [nSentences];
	wcscpy(text, _text);
	
	nSentences = 0;
	currentSentence = wcstok(text, delim, &pt);
	while (currentSentence != nullptr) {
		sentences[nSentences] = new wchar_t[wcslen(currentSentence) + 1];
		wcscpy(sentences[nSentences], currentSentence);
		nSentences++;

		currentSentence = wcstok(nullptr, delim, &pt);
	}

	delete[] text;
	return sentences;
}
