#include "Sort.h"
#include <wchar.h>

void assign(void* dst, void* src, int nBytes)
{
	unsigned char* it = (unsigned char*)src;
	unsigned char* it2 = (unsigned char*)dst;
	for (int i = 0; i < nBytes; i++, it++, it2++)
		*it2 = *it;
}

void copyArray(void* src, void* dest, int nElem, int nBytes)
{
	unsigned char* it0 = (unsigned char*)dest;
	unsigned char* it1 = (unsigned char*)src;

	for (int i = 0; i < nElem; i++, it0 += nBytes, it1 += nBytes)
		assign((void*)it0, (void*)it1, nBytes);
}

void mergeArrays(void* a, void* b, int nA, int nB, int nBytes, f cmp, void *result)
{
	int i = 0;
	unsigned char* it = (unsigned char*)a;

	int j = 0;
	unsigned char* jt = (unsigned char*)b;

	int k = 0;
	unsigned char* kt = (unsigned char*)result;

	for (int i = 0; i < nA; i++, it += nBytes) {
		while (j < nB && cmp(jt, it)) {
			assign((void*)kt, (void*)jt, nBytes);

			k++;
			kt += nBytes;

			j++;
			jt += nBytes;
		}

		assign(kt, it, nBytes);
		k++;
		kt += nBytes;
	}

	while (j < nB) {
		assign((void*)kt, (void*)jt, nBytes);

		k++;
		kt += nBytes;

		j++;
		jt += nBytes;
	}
}

void mergeSortRecursive(void* a, int nElem, int nBytes, f cmp, void* buffer)
{
	if (nElem <= 1)
		return;

	int nElem1 = nElem / 2;
	int nElem2 = nElem - nElem1;

	void* a1 = a;
	void* a2 = (void*)(((unsigned char*)a) + nElem1 * nBytes);

	mergeSortRecursive(a1, nElem1, nBytes, cmp, buffer);
	mergeSortRecursive(a2, nElem2, nBytes, cmp, buffer);

	mergeArrays(a1, a2, nElem1, nElem2, nBytes, cmp, buffer);
	copyArray(buffer, a, nElem, nBytes);
}

void mergeSort(void* a, int nElem, int nBytes, f cmp)
{
	unsigned char* buffer = new unsigned char[nElem * nBytes];
	unsigned char* it = (unsigned char*)a;
	mergeSortRecursive(a, nElem, nBytes, cmp, (void*)buffer);
	delete[] buffer;
}

bool intCmp(void* lhs, void* rhs)
{
	int lhsVal = *((int*)lhs);
	int rhsVal = *((int*)rhs);

	return lhsVal < rhsVal;
}

