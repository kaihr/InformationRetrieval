#include"Header.h"


int main()
{
    int stops[100];
    Stop_word stopwords[MAX_NUM_STOPWORDS];
    int num_words = 0, total = 0;

    ReadStopWordFromFile("stop_words.txt", stopwords, num_words);

    string c = ReadLineFromStory("text.txt");


    ofstream fout;
    fout.open("text_cleaned.txt");

    total = RemoveAllStopwordsFromLine(c, stops, stopwords, num_words) + 1;

    fout << c;
    fout.close();


    WriteStopWordCountToFile("stop_words_count.txt", stops, stopwords, num_words);

    cout << "Number of stop words removed = " << total << endl;

    return 0;
}