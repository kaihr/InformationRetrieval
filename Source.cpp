#include "SimpleSearchEngine.h"
#include "TextProcessing.h"
#include "TextNormalizer.h"
#include "Updater.h"
#include "Sort.h"
#include "FastInputOutput.h"
#include "Lazy.h"

#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <wchar.h>
#include <Windows.h>
#include <time.h>
#include <locale.h>

int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);

	srand(time(nullptr));

	if (isFirstRun()) {
		buildHashTable(L"new train");
		wprintf(L"\nPlease re-start the program.\n");
		getchar();
		return 0;
	}

	if (!init()) {
		wprintf(L"Can not load inverted index\n");
		wprintf(L"Press any button to continue\n");
		getchar();
		return 0;
	}

	loadDocFreq(L"doc_freq.txt");
	calcPos(L"inverted_index.txt");

	wchar_t pathDelim[] = L"\xfeff\n";
	int nFilesRead = 0;
	wchar_t* paths = readFile(L"path.txt");

	if (!paths) {
		wprintf(L"Can not load directory list\n");
		wprintf(L"Press any button to continue\n");
		getchar();
		return 0;
	}

	wchar_t** pathList = splitToken(paths, nFilesRead, pathDelim);
	int nFiles = nFilesRead / 2;

	int nStopWords = 0;
	wchar_t* text = readFile(L"resources\\vietnamese_stopwords.txt");

	if (!text) {
		wprintf(L"Can not load stopwords\n");
		wprintf(L"Press any button to continue\n");
		getchar();
		return 0;
	}

	wchar_t** stopWords = sentenceToken(text, nStopWords);
	mergeSort((void*)stopWords, nStopWords, sizeof(wchar_t*), wchComp);

	wchar_t userInput[256] = L"";

	delete[] paths;
	delete[] text;

	while (true) {
		system("cls");
		wprintf(L"=== INFORMATION RETRIEVAL 69420 ===\n");
		wprintf(L"1. Search\n");
		wprintf(L"2. Add/Update a file to search engine\n");
		wprintf(L"3. Remove a file from search engine\n");
		wprintf(L"4. Quit\n");

		int choice;
		wprintf(L"Enter your choice: ");
		wscanf(L"%d", &choice);

		if (choice == 4) {
			break;
		}

		if (choice == 1) {
			setbuf(stdin, nullptr);
			wprintf(L"Searching for?: ");
			fgetws(userInput, 256, stdin);

			find100Best(pathList, nFiles, userInput);
		}

		if (choice == 2 || choice == 3) {
			fgetws(userInput, 256, stdin);

			if (choice == 3)
				wprintf(L"Enter file's ABSOLUTE path to add/update (no leading or trailing spaces): ");
			else
				wprintf(L"Enter file's ABSOLUTE path to remove (no leading or trailing spaces): ");
			
			fgetws(userInput, 256, stdin);
			userInput[wcslen(userInput) - 1] = '\0';

			time_t startTime = clock();

			if (choice == 2) {
				wprintf(L"Adding %ls to search engine...\n", userInput);
				remFile(nFiles, userInput, L"path.txt", pathList, stopWords, nStopWords);
				if (!addFile(nFiles, userInput, L"path.txt", pathList, stopWords, nStopWords))
					wprintf(L"Can not add %ls to search engine\n", userInput);
				else
					wprintf(L"Done. Elapsed time: %.5lf seconds\n", (clock() - startTime) * 1.00 / CLOCKS_PER_SEC);
			}

			if (choice == 3) {
				wprintf(L"Removing %ls from search engine...\n", userInput);
				if (!remFile(nFiles, userInput, L"path.txt", pathList, stopWords, nStopWords))
					wprintf(L"Remove failed, %ls did not exist in the search engine\n", userInput);
				else
					wprintf(L"Done. Elapsed time: %.5lf seconds\n", (clock() - startTime) * 1.00 / CLOCKS_PER_SEC);
			}

			setbuf(stdin, nullptr);
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

	wprintf(L"Saving...\n");
	
	saveInvIndex(L"inverted_index.txt");
	releaseInvTable();

	saveDocFreq(L"doc_freq.txt");

	return 0;
}