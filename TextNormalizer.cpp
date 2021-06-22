#include "TextNormalizer.h"
#include <stdio.h>
#include <wchar.h>

int isIn(wchar_t ch, const wchar_t* set)
{
	int n = wcslen(set);

	for (int i = 0; i < n; i++)
		if (ch == set[i])
			return i;

	return -1;
}

wchar_t toLowerCase(wchar_t ch)
{
	if ('a' <= ch && ch <= 'z')
		return ch;

	if ('A' <= ch && ch <= 'Z')
		return ch - 'A' + 'a';

	int foo;
	wchar_t a[] = L"àáảãạăằắẳẵặâầấẩẫậÀÁẢÃẠĂẰẮẲẴẶÂẦẤẨẪẬ";
	wchar_t d[] = L"đĐ";
	wchar_t e[] = L"èéẻẽẹêềếểễệÈÉẺẼẸÊỀẾỂỄỆ";
	wchar_t i[] = L"ìíỉĩịÌÍỈĨỊ";
	wchar_t o[] = L"òóỏõọôồốổỗộơờớởỡợÒÓỎÕỌÔỒỐỔỖỘƠỜỚỞỠỢ";
	wchar_t u[] = L"ùúủũụưừứửữựÙÚỦŨỤƯỪỨỬỮỰ";
	wchar_t y[] = L"ỳýỷỹỵỲÝỶỸỴ";

	if ((foo = isIn(ch, a)) != -1) {
		int half = wcslen(a) / 2;
		if (foo < half)
			return a[foo];
		else
			return a[foo - half];
	}

	if ((foo = isIn(ch, d)) != -1) {
		int half = wcslen(d) / 2;
		if (foo < half)
			return d[foo];
		else
			return d[foo - half];
	}

	if ((foo = isIn(ch, e)) != -1) {
		int half = wcslen(e) / 2;
		if (foo < half)
			return e[foo];
		else
			return e[foo - half];
	}

	if ((foo = isIn(ch, i)) != -1) {
		int half = wcslen(i) / 2;
		if (foo < half)
			return i[foo];
		else
			return i[foo - half];
	}

	if ((foo = isIn(ch, o)) != -1) {
		int half = wcslen(o) / 2;
		if (foo < half)
			return o[foo];
		else
			return o[foo - half];
	}

	if ((foo = isIn(ch, u)) != -1) {
		int half = wcslen(u) / 2;
		if (foo < half)
			return u[foo];
		else
			return u[foo - half];
	}

	if ((foo = isIn(ch, y)) != -1) {
		int half = wcslen(y) / 2;
		if (foo < half)
			return y[foo];
		else
			return y[foo - half];
	}

	if(ch >= 128)
		return ' ';

	return ch;
}

wchar_t toLatinLetter(wchar_t ch)
{
	wchar_t a[] = L"àáảãạăằắẳẵặâầấẩẫậÀÁẢÃẠĂẰẮẲẴẶÂẦẤẨẪẬ";
	wchar_t d[] = L"đĐ";
	wchar_t e[] = L"èéẻẽẹêềếểễệÈÉẺẼẸÊỀẾỂỄỆ";
	wchar_t i[] = L"ìíỉĩịÌÍỈĨỊ";
	wchar_t o[] = L"òóỏõọôồốổỗộơờớởỡợÒÓỎÕỌÔỒỐỔỖỘƠỜỚỞỠỢ";
	wchar_t u[] = L"ùúủũụưừứửữựÙÚỦŨỤƯỪỨỬỮỰ";
	wchar_t y[] = L"ỳýỷỹỵỲÝỶỸỴ";

	if ('a' <= ch && ch <= 'z')
		return ch;
	if ('A' <= ch && ch <= 'Z')
		return ch - 'A' + 'a';
	if (isIn(ch, a) != -1)
		return L'a';
	if (isIn(ch, d) != -1)
		return L'd';
	if (isIn(ch, e) != -1)
		return L'e';
	if (isIn(ch, i) != -1)
		return L'i';
	if (isIn(ch, o) != -1)
		return L'o';
	if (isIn(ch, u) != -1)
		return L'u';
	if (isIn(ch, y) != -1)
		return L'y';

	if (ch >= 128)
		return ' ';

	return ch;
}

void toLowerCase(wchar_t* str)
{
	for (int i = 0, nChar = wcslen(str); i < nChar; i++)
		str[i] = toLowerCase(str[i]);
}

void toLatinLetter(wchar_t* str)
{
	for (int i = 0, nChar = wcslen(str); i < nChar; i++)
		str[i] = toLatinLetter(str[i]);
}
