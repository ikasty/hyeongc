#ifndef VALUE_H
#define VALUE_H

#include "code.h"

struct Value *getNewValue ();
struct Value *ValueAdd (struct Value *const, const struct Value *);
struct Value *ValueMul (struct Value *const, const struct Value *);
void ValueNeg (struct Value *const);
void ValueInv (struct Value *const);
struct Value *ValueCopy (const struct Value *);

int ValueLess (const struct Value *, const long long);
int ValueEqual (const struct Value *, const long long);

#endif