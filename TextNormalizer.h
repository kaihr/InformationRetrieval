#pragma once

bool isIn(wchar_t ch, int set[], int nElem);
wchar_t wideCharNormalize(wchar_t ch);
bool textNormalize(const wchar_t* inputPath, const wchar_t* outputPath);
bool readWithOutGiveAShit(const wchar_t* inputPath);