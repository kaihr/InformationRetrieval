#pragma once
wchar_t* readFile(const wchar_t* inputPath);
wchar_t** sentenceToken(const wchar_t* text, int &nSentences);
wchar_t** wordToken(const wchar_t* text, int &nWords);