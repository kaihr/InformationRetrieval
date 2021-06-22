#include "Pair.h"

bool pairCmp(void* lhs, void* rhs)
{
	Pair lhsVal = *((Pair*)lhs);
	Pair rhsVal = *((Pair*)rhs);

	if (lhsVal.a != rhsVal.a)
		return lhsVal.a > rhsVal.a;

	return lhsVal.b < rhsVal.b;
}
