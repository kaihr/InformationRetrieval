#include "TextNormalizer.h"
#include "windows.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"

int nFilesRead = 0;

bool listDirectoryContent(const wchar_t* sDir)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    wchar_t sPath[2048];
    wchar_t oPath[2048];

    wsprintf(sPath, L"%s\\*.*", sDir);

    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        wprintf(L"Path not found: [%s]\n", sDir);
        return false;
    }

    do
    {
        if (wcscmp(fdFile.cFileName, L".") != 0 && wcscmp(fdFile.cFileName, L"..") != 0)
        {
            swprintf(sPath, L"%s\\%s", sDir, fdFile.cFileName);
            if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                wprintf(L"Directory: %s\n", sPath);
                listDirectoryContent(sPath);
            }
            else {
                swprintf(oPath, L"%s%s", L"compressed\\", fdFile.cFileName);
                //wprintf(L"%s\n", oPath);
                
                if (readWithOutGiveAShit(sPath))
                    nFilesRead++;
            }
        }
    }     while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);

    return true;
}

int main() {
    time_t beginTime = clock();

    listDirectoryContent(L"new train");

    fprintf(stderr, "Read %d files.\n", nFilesRead);
    fprintf(stderr, "Elapsed time: %.5lf.\n", 1.0 * (clock() - beginTime) / CLOCKS_PER_SEC);

	return 0;
}