#include "StringMatching.h"

#include <wchar.h>

bool isMatched(const wchar_t* haystack, const wchar_t* needle)
{
	int n = wcslen(haystack);
	int m = wcslen(needle);

	for (int startPos = 0; startPos + m - 1 < n; startPos++) {
		if ((startPos != 0) && (haystack[startPos - 1] != ' '))
			continue;

		bool foundAtCurPos = true;
		for (int i = 0; i < m; i++) {
			if (haystack[startPos + i] != needle[i]) {
				foundAtCurPos = false;
				break;
			}
		}

		if (foundAtCurPos) {
			int finishPos = startPos + m - 1;
			if ((finishPos == n - 1) || (haystack[finishPos + 1] == L' '))
				return true;
		}
	}

	return false;
}
