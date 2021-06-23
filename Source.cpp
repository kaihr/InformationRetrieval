#include "SimpleSearchEngine.h"
#include "TextProcessing.h"
#include "TextNormalizer.h"
#include "Updater.h"
#include "Sort.h"

#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <wchar.h>
#include <Windows.h>
#include <time.h>

int main() {
	srand(time(nullptr));
	//buildHashTable(L"new train");
	//return 0;

	_setmode(_fileno(stdout), _O_U16TEXT);

	if (!init()) {
		wprintf(L"Can not load inverted index\n");
		wprintf(L"Press any button to continue\n");
		getchar();
		return 0;
	}

	loadInvTable(L"inverted_index.txt");

	wchar_t pathDelim[] = L"\xfeff\n";
	int nFilesRead = 0;
	wchar_t* paths = readFile(L"path.txt");
	wchar_t** pathList = splitToken(paths, nFilesRead, pathDelim);

	int nFiles = nFilesRead / 2;

	int nStopWords = 0;
	wchar_t* text = readFile(L"resources\\vietnamese_stopwords.txt");
	wchar_t** stopWords = sentenceToken(text, nStopWords);
	mergeSort((void*)stopWords, nStopWords, sizeof(wchar_t*), wchComp);

	wchar_t userInput[256];

	while (true) {
		system("cls");
		wprintf(L"=== SHITTY SEARCH ENGINE ===\n");
		wprintf(L"1. Search\n");
		wprintf(L"2. Add/Update files to search engine\n");
		wprintf(L"3. Remove files from search engine\n");
		wprintf(L"4. Quit\n");

		int choice;
		wprintf(L"Enter your choice: ");
		wscanf(L"%d", &choice);

		if (choice == 4) {
			break;
		}

		if (choice == 1) {
			fgetws(userInput, 256, stdin);
			wprintf(L"Searching for?: ");
			fgetws(userInput, 256, stdin);

			find100Best(pathList, nFiles, userInput);
		}

		if (choice == 2 || choice == 3) {
			fgetws(userInput, 256, stdin);

			if (choice == 3)
				wprintf(L"Enter file's path to add/update (no leading or trailing spaces): ");
			else
				wprintf(L"Enter file's path to remove (no leading or trailing spaces): ");
			
			fgetws(userInput, 256, stdin);
			userInput[wcslen(userInput) - 1] = '\0';

			if (choice == 2) {
				wprintf(L"Adding %ls to search engine...\n", userInput);
				remFile(nFiles, userInput, L"path.txt", pathList, stopWords, nStopWords);
				if (!addFile(nFiles, userInput, L"path.txt", pathList, stopWords, nStopWords))
					wprintf(L"Can not add %ls to search engine\n", userInput);
				else
					wprintf(L"Done\n");
			}

			if (choice == 3) {
				wprintf(L"Removing %ls from search engine...\n", userInput);
				if (!remFile(nFiles, userInput, L"path.txt", pathList, stopWords, nStopWords))
					wprintf(L"Remove failed, %ls did not exist in the search engine\n", userInput);
				else
					wprintf(L"Done\n");
			}

			wprintf(L"Press any button to continue\n");
			getchar();
		}
	}
	
	for (int i = 0; i < nStopWords; i++)
		delete[] stopWords[i];
	delete[] stopWords;
	
	for (int i = 0; i < 2 * nFiles; i++)
		delete[] pathList[i];

	delete[] pathList;
	delete[] paths;
	delete[] text;
	releaseInvTable();

	return 0;
}