#ifndef VALUE_H
#define VALUE_H

struct Value {
	int nan;
	long long top, bottom;
	struct Value *stackp;
} VALUE_NULL;

struct Value *getNewValue ();
struct Value *ValueAdd (struct Value *const, const struct Value *);
struct Value *ValueMul (struct Value *const, const struct Value *);
void ValueNeg (struct Value *const);
void ValueInv (struct Value *const);
struct Value *ValueCopy (const struct Value *);

#endif