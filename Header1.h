#pragma once

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int MAX_NUM_STOPWORDS = 100;

struct Stop_word
{
    string word;  
    int count;   
};

string ReadLineFromStory(const char* Filename);

void ReadStopWordFromFile(const char* stop_word_filename, Stop_word words[], int& num_words);

void WriteStopWordCountToFile(const char* wordcount_filename, int stops[], Stop_word words[], int num_words);

int RemoveWordFromLine(string& line, string word);

int RemoveAllStopwordsFromLine(string& line, int stops[], Stop_word words[], int num_words);
