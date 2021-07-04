#include "Lazy.h"
#include "Constant.h"
#include "FastInputOutput.h"

#include <stdio.h>
#include <stdint.h>

int64_t posInFile[BUCKET_SIZE];
bool loaded[BUCKET_SIZE];

void calcPos(const wchar_t* inputPath)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");

	for (int i = 0; i < BUCKET_SIZE; i++) {
		if (i % (BUCKET_SIZE / 10) == 0)
			wprintf(L"Loading...\n");

		posInFile[i] = ftell(fin);

		int size = 0;
		size = readInt(fin);

		while (size-- > 0) {
			int x = 0;
			x = readInt(fin);
		}
	}

	fclose(fin);
}

void loadListFromLine(LinkedList* ls, const wchar_t* inputPath, int lineNum)
{
	if (loaded[lineNum])
		return;

	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");
	fseek(fin, posInFile[lineNum], SEEK_SET);

	int size = 0;
	size = readInt(fin);

	while (size-- > 0) {
		int x = 0;
		x = readInt(fin);
		insertBack(ls, x);
	}

	loaded[lineNum] = true;
	fclose(fin);
}

void loadAll()
{
	for (int i = 0; i < BUCKET_SIZE; i++)
		loaded[i] = true;
}
