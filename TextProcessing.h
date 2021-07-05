#pragma once
wchar_t* readFile(const wchar_t* inputPath);

wchar_t** splitToken(const wchar_t* text, int& nTok, const wchar_t *delim);

wchar_t** sentenceToken(const wchar_t* text, int &nSentences);
wchar_t** wordToken(const wchar_t* text, int &nWords);

double getDocNorm(const wchar_t* inputPath, int nFiles);