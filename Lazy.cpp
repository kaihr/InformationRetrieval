#include "Lazy.h"
#include "Constant.h"
#include "FastInputOutput.h"

#include <stdio.h>
#include <stdint.h>

int64_t posInFile[NUMBER_OF_TERMS];
int df[NUMBER_OF_TERMS];

bool calcPos(const wchar_t* inputPath)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");

	if (!fin)
		return false;

	for (int i = 0; i < NUMBER_OF_TERMS; i++) {
		if (i % (NUMBER_OF_TERMS / 10) == 0)
			wprintf(L"Loading...\n");

		posInFile[i] = ftell(fin);

		int size = 0;
		size = readInt(fin);

		while (size-- > 0) {
			int x = 0, freq = 0;
			x = readInt(fin);
			freq = readInt(fin);
		}
	}

	fclose(fin);
	return true;
}

bool loadDocFreq(const wchar_t* fileName)
{
	FILE* fin = _wfopen(fileName, L"r,ccs=UTF-8");

	if (!fin)
		return false;

	for (int i = 0; i < NUMBER_OF_TERMS; i++) {
		df[i] = readInt(fin);
		df[i] = -df[i];
	}

	return true;
}

void loadListFromLine(LinkedList* ls, const wchar_t* inputPath, int lineNum)
{
	if (df[lineNum] >= 0)
		return;

	df[lineNum] *= -1;

	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");
	fseek(fin, posInFile[lineNum], SEEK_SET);

	int size = 0;
	size = readInt(fin);

	while (size-- > 0) {
		int x = 0, freq = 0;
		x = readInt(fin);
		freq = readInt(fin);
		insertBack(ls, x, freq);
	}

	fclose(fin);
}

int getDocFreq(int termID)
{
	return df[termID];
}

void setDocFreq(int termID, int value)
{
	df[termID] = value;
}

bool saveDocFreq(const wchar_t* fileName)
{
	FILE* fout = _wfopen(fileName, L"w,ccs=UTF-8");

	if (!fout)
		return false;

	for (int i = 0; i < NUMBER_OF_TERMS; i++)
		fwprintf(fout, L"%d ", df[i]);

	fclose(fout);
	return true;
}

void initDocFreq()
{
	for (int i = 0; i < NUMBER_OF_TERMS; i++)
		df[i] = 0;
}
