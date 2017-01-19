#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "code.h"
#include "value.h"

struct Stack {
	int stack_value;
	struct Stack *next;
	struct Value *value;
} STACK_NULL;

void interpret(struct Code *, int, int);

#endif
