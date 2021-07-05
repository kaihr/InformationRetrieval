#include "StringMatching.h"

#include <wchar.h>

bool isMatched(const wchar_t* haystack, const wchar_t* needle)
{
	int n = wcslen(haystack);
	int m = wcslen(needle);

	for (int startPos = 0; startPos + m < n; startPos++) {
		bool foundAtCurPos = true;
		for (int i = 0; i < m; i++) {
			if (haystack[startPos + i] != needle[i]) {
				foundAtCurPos = false;
				break;
			}
		}

		if (foundAtCurPos)
			return true;
	}

	return false;
}
