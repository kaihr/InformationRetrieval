#include "StopWords.h"
#include "stdio.h"
#include "wchar.h"
#include "assert.h"

wchar_t** loadStopWords(const wchar_t* inputPath, int &nStrings)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");

	if (!fin)
		return nullptr;

	wchar_t buffer[100] = { 0 };
	nStrings = 0;

	while (fgetws(buffer, 100, fin))
		nStrings++;

	fseek(fin, 0, SEEK_SET);

	fprintf(stderr, "n = %d", nStrings);

	wchar_t** stopWords = new wchar_t*[nStrings];
	int iter = 0;

	while (fgetws(buffer, 100, fin)) {
		stopWords[iter] = new wchar_t[wcslen(buffer) + 1];
		wcscpy(stopWords[iter], buffer);
		iter++;
	}

	assert(iter == nStrings);

	fclose(fin);
	return stopWords;
}
