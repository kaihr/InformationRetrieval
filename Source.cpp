#include "TextNormalizer.h"
#include "TextProcessing.h"
#include "StopWords.h"

#include "windows.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"

int main() {
	wchar_t* input = readFile(L"new train\\Am nhac\\AN_TN_ (878).txt");

	sentenceToken(input);

	delete[] input;

	return 0;
}