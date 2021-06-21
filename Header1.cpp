#include"Header.h"

string ReadLineFromStory(const char* Filename)
{
    string x = "";
    string b;
    ifstream fin;
    fin.open(Filename);
    while (getline(fin, b))
    {
        x += b;
    }
    return x;
}

void ReadStopWordFromFile(const char* stop_word_filename, Stop_word words[], int& num_words)
{
    ifstream fin;
    fin.open(stop_word_filename); // chua co
    string a;
    int i = 0;
    if (fin.fail())
    {
        cout << "Failed to open " << stop_word_filename << endl;
        exit(1);
    }
    words[num_words].count = 0;
    while (fin >> words[num_words].word)
    {
        ++num_words;
    }

    fin.close();
}

void WriteStopWordCountToFile(const char* wordcount_filename, int stops[], Stop_word words[], int num_words)
{
    ofstream fout;
    fout.open(wordcount_filename);

    for (int i = 0; i < num_words; i++)
    {
        fout << words[i].word << " " << stops[i] << endl;
    }

    fout.close();
}

int RemoveWordFromLine(string& line, string word)
{
    int length = line.length();
    int counter = 0;
    int wl = word.length();
    for (int i = 0; i < length; i++)
    {
        int x = 0;
        if (line[i] == word[0] && (i == 0 || (i != 0 && line[i - 1] == ' ')))
        {
            for (int j = 1; j < wl; j++)
                if (line[i + j] != word[j])
                {
                    x = 1;
                    break;
                }
            if (x == 0 && (i + wl == length || (i + wl != length && line[i + wl] == ' ')))
            {
                for (int k = i + wl; k < length; k++)
                    line[k - wl] = line[k];
                length -= wl;

                counter++;
            }
        }

    }
    line[length] = 0;
    char newl[1000] = { 0 };
    for (int i = 0; i < length; i++)
        newl[i] = line[i];
    line.assign(newl);
    return counter;
}

int RemoveAllStopwordsFromLine(string& line, int stops[], Stop_word words[], int num_words)
{
    int counter[100];
    int final = 0;
    for (int i = 0; i < num_words; i++)
    {
        counter[i] = RemoveWordFromLine(line, words[i].word);
        final += counter[i];
        stops[i] = counter[i];

    }
    return final;

}