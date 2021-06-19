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

void wordToken(const wchar_t* _text)
{
	int length = wcslen(_text);
	wchar_t* text = new wchar_t[length + 1];
	wcscpy(text, _text);

	wchar_t* currentWord;
	wchar_t* pt;

	wchar_t delim[] = L" ";

	currentWord = wcstok(text, delim, &pt);

	bool first = true;

	while (currentWord != nullptr) {
		if (!first)
			wprintf(L" ");
		first = false;

		wprintf(L"%ls", currentWord);
		currentWord = wcstok(nullptr, delim, &pt);
	}

	delete[] text;
}

void sentenceToken(const wchar_t* _text)
{
	int length = wcslen(_text);
	wchar_t* text = new wchar_t[length + 1];
	wcscpy(text, _text);

	wchar_t* currentSentence;
	wchar_t* pt;

	wchar_t delim[] = L".,:;'\"!()\n";

	currentSentence = wcstok(text, delim, &pt);
	while (currentSentence != nullptr) {
		//wprintf(L"%ls\n", currentSentence);
		currentSentence = wcstok(nullptr, delim, &pt);
		wordToken(currentSentence);
		wprintf(L"\n");
	}

	delete[] text;
}
