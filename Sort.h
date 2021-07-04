#pragma once

typedef bool (*f)(void* lhs, void* rhs);

void assign(void* dst, void* src, int nBytes);
void copyArray(void* src, void* dest, int nElem, int nBytes);

void mergeArrays(void* a, void* b, int nA, int nB, int nBytes, f cmp, void *result);
void mergeSortRecursive(void* a, int nElem, int nBytes, f cmp, void* buffer);
void mergeSort(void* a, int nElem, int nBytes, f cmp);