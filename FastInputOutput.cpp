#include "FastInputOutput.h"

int readInt(FILE* fin)
{
	bool isNeg = false;
	int ans = 0;

	char c;
	for (c = getwc(fin); (c < '0' || c > '9') && !feof(fin); c = getwc(fin))
		if (c == '-')
			isNeg = true;

	ans = (c - '0');
	for (c = getwc(fin); ('0' <= c && c <= '9') && !feof(fin); c = getwc(fin))
		ans = ans * 10 + (c - '0');

	if (isNeg)
		ans = -ans;

	return ans;
}

void writeInt(FILE* fout, int num) {
	if (num < 0)
	{
		putchar('-');
		num = -num;
	}
	
	int p = 1;
	for (int temp = num / 10; temp > 0; temp /= 10) p *= 10;
	for (; p > 0; num %= p, p /= 10) putwc((num / p) + '0', fout);
}
