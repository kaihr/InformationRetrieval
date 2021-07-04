#include "Tuple.h"

bool tupleCmp(void* lhs, void* rhs)
{
	Tuple lhsTup = *((Tuple*)lhs);
	Tuple rhsTup = *((Tuple*)rhs);

	if(lhsTup.a != rhsTup.a)
		return lhsTup.a < rhsTup.a;

	if (lhsTup.b != rhsTup.b)
		return lhsTup.b < rhsTup.b;

	return lhsTup.c < rhsTup.c;
}
