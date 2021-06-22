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

int main() {
	//buildHashTable(L"new train");

	_setmode(_fileno(stdout), _O_U16TEXT);

	init();
	int nFiles = loadInvTable(L"inverted_index.txt");
	

	wchar_t* paths = readFile(L"path.txt");
	int nFilesRead = 0;
	wchar_t pathDelim[] = L"\xfeff\n";
	wchar_t** pathList = splitToken(paths, nFilesRead, pathDelim);

	wchar_t userInput[256];

	while (true) {
		system("cls");
		wprintf(L"=== SHITTY SEARCH ENGINE ===\n");
		wprintf(L"1. Search\n");
		wprintf(L"2. Quit\n");

		int choice;
		wscanf(L"%d", &choice);

		if (choice == 2) {
			break;
		}


		if (choice == 1) {
			fgetws(userInput, 256, stdin);
			wprintf(L"Searching for?: ");
			fgetws(userInput, 256, stdin);

			find100Best(pathList, nFiles, userInput);
		}
	}




	for (int i = 0; i < nFiles; i++)
		delete[] pathList[i];
	delete[] pathList;
	delete[] paths;
	releaseInvTable();

	return 0;
}