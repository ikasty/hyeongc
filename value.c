#include <stdlib.h>
#include <math.h>
#include "value.h"

long long getGCD (long long a, long long b)
{
	while (b)
	{
		long long t = b;
		b = a % b;
		a = t;
	}
	return a;
}

struct Value *getNewValue ()
{
	struct Value *v = malloc(sizeof(struct Value));
	*v = VALUE_NULL;
	v->bottom = 1;
	return v;
}

struct Value *ValueAdd (struct Value *const a, const struct Value *b)
{
	if (a->nan || b->nan) {
		a->nan = 1;
		return a;
	}

	long long c = getGCD(a->bottom, b->bottom);
	long long atop = a->bottom / c * a->top;
	long long btop = b->bottom / c * b->top;

	a->bottom = a->bottom / c * b->bottom;
	a->top = atop + btop;

	c = getGCD(abs(a->top), a->bottom);
	a->top /= c; a->bottom /= c;
	return a;
}

struct Value *ValueMul (struct Value *const a, const struct Value *b)
{
	if (a->nan || b->nan) {
		a->nan = 1;
		return a;
	}

	long long c = getGCD(abs(a->top), b->bottom);
	long long atop = a->top / c, bbot = b->bottom / c;

	c = getGCD(a->bottom, abs(b->top));
	long long btop = b->top / c, abot = a->bottom / c;

	a->bottom = abot * bbot;
	a->top = atop * btop;

	return a;
}

void ValueNeg (struct Value *const a)
{
	a->top *= -1;
}

void ValueInv (struct Value *const a)
{
	long long t = a->top;
	a->top = a->bottom;
	a->bottom = t;
	if (!a->bottom) a->nan = 1;
	if (a->bottom < 0) a->bottom *= -1, a->top *= -1;
}

struct Value *ValueCopy (const struct Value *a)
{
	struct Value *v = getNewValue();
	*v = *a;
	return v;
}

int ValueLess (const struct Value *a, const long long x)
{
	if (a->nan) return 0;
	long long avalue = a->top / a->bottom;
	return avalue < x;
}

int ValueEqual (const struct Value *a, const long long x)
{
	if (a->nan) return 0;
	long long avalue = a->top / a->bottom;
	return avalue == x;
}
