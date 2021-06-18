#include "TextNormalizer.h"
#include "stdio.h"

bool isIn(wchar_t ch, int set[], int nElem) {
	for (int i = 0; i < nElem; i++)
		if (ch == set[i])
			return true;

	return false;
}

wchar_t wideCharNormalize(wchar_t ch)
{
	int a[] = { 97, 224, 225, 7843, 227, 7841, 259, 7857, 7855, 7859, 7861, 7863, 226, 7847, 7845, 7849, 7851, 7853, 192, 193, 7842, 195, 7840, 258, 7856, 7854, 7858, 7860, 7862, 194, 7846, 7844, 7848, 7850, 7852 };
	int d[] = { 100, 273, 272 };
	int e[] = { 101, 232, 233, 7867, 7869, 7865, 234, 7873, 7871, 7875, 7877, 7879, 200, 201, 7866, 7868, 7864, 202, 7872, 7870, 7874, 7876, 7878 };
	int i[] = { 105, 236, 237, 7881, 297, 7883, 204, 205, 7880, 296, 7882 };
	int o[] = { 111, 242, 243, 7887, 245, 7885, 244, 7891, 7889, 7893, 7895, 7897, 417, 7901, 7899, 7903, 7905, 7907, 210, 211, 7886, 213, 7884, 212, 7890, 7888, 7892, 7894, 7896, 416, 7900, 7898, 7902, 7904, 7906 };
	int u[] = { 117, 249, 250, 7911, 361, 7909, 432, 7915, 7913, 7917, 7919, 7921, 217, 218, 7910, 360, 7908, 431, 7914, 7912, 7916, 7918, 7920 };
	int y[] = { 121, 7923, 253, 7927, 7929, 7925, 7922, 221, 7926, 7928, 7924 };

	if ('a' <= ch && ch <= 'z')
		return ch;
	if ('A' <= ch && ch <= 'Z')
		return ch - 'A' + 'a';
	if (isIn(ch, a, 35))
		return L'a';
	if (isIn(ch, d, 3))
		return L'd';
	if (isIn(ch, e, 23))
		return L'e';
	if (isIn(ch, i, 11))
		return L'i';
	if (isIn(ch, o, 35))
		return L'o';
	if (isIn(ch, u, 23))
		return L'u';
	if (isIn(ch, y, 11))
		return L'y';

	return ch;
}

bool textNormalize(const wchar_t* inputPath, const wchar_t* outputPath)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");
	FILE* fout = _wfopen(outputPath, L"wb+,ccs=UTF-8");

	if (!fin)
		return false;

	if (!fout)
		return false;

	wchar_t last = L' ';
	while (true) {
		wchar_t ch = fgetwc(fin);

		if (feof(fin))
			break;

		ch = wideCharNormalize(ch);

		if (ch == ' ' || ch == '\n') {
			if (ch == last)
				continue;
		}

		last = ch;
		fwprintf(fout, L"%lc", ch);
	}

	fclose(fin);
	fclose(fout);

	return true;
}

bool readWithOutGiveAShit(const wchar_t* inputPath)
{
	FILE* fin = _wfopen(inputPath, L"r,ccs=UTF-8");

	if (!fin)
		return false;

	wchar_t last = L' ';
	while (true) {
		wchar_t ch = fgetwc(fin);

		if (feof(fin))
			break;

		ch = wideCharNormalize(ch);

		if (ch == ' ' || ch == '\n') {
			if (ch == last)
				continue;
		}

		last = ch;
	}

	fclose(fin);

	return true;
}
