#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "debug.h"
#include "value.h"

struct Stack {
	int stack_value;
	struct Stack *next;
	struct Value *value;
} STACK_NULL;

struct Pointer {
	struct Code *pos;
};

struct Stack *Stack_Hash[10], *stack_stdin, *stack_stdout, *stack_stderr, *stack_current;
struct Pointer Pointers[11], *ref;

struct Stack *getStack (int d)
{
	if (d == -1) {
		struct Stack *tempstack = malloc(sizeof(struct Stack));
		*tempstack = STACK_NULL;
		tempstack->stack_value = -1;
		return tempstack;
	}

	struct Stack *s = Stack_Hash[d % 10];
	while (s->stack_value != d) s = s->next;
	if (!s) {
		struct Stack *newstack = malloc(sizeof(struct Stack));
		*newstack = STACK_NULL;
		newstack->stack_value = d;
		Stack_Hash[d % 10] = newstack->next = Stack_Hash[d % 10];
		s = newstack;
	}
	return s;
}

void push (struct Stack *s, struct Value *v)
{
	if (s == stack_stdin) {
	} else if (s == stack_stdout) {
		if (v->nan) printf("너무 커엇...");
		else if (v->top >= 0) putchar(v->top / v->bottom); // unicode change
		else printf("%lld", -v->top / v->bottom);
	} else if (s == stack_stderr) {
		if (v->nan) fprintf(stderr, "너무 커엇...");
		else if (v->top > 0) putc(v->top / v->bottom, stderr); // unicode change
		else fprintf(stderr, "%lld", -v->top / v->bottom);
	} else {
		v->stackp = s->value;
		s->value = v;
	}
}

struct Value *pop (struct Stack *s)
{
	struct Value *v;
	if (s == stack_stdin) {
		// stack_stdin control
	} else if (s == stack_stdout) {
		exit(0);
	} else if (s == stack_stderr) {
		exit(1);
	} else {
		v = s->value;
		if (!v) {
			v = getNewValue();
			v->nan = 1;
		} else {
			s->value = s->value->stackp;
		}
	}
	return v;
}

void Op_Push (struct Code *code, struct Heart_Tree *tree)
{
	struct Value *v = getNewValue();
	v->top = code->charcnt * code->dotcnt;
	v->bottom = 1;
	push(stack_current, v);
}

void Op_Add (struct Code *code, struct Heart_Tree *tree)
{
	struct Value *v = pop(stack_current), *p;
	for (int i = 1; i < code->charcnt; i++)
	{
		p = pop(stack_current);
		v = ValueAdd(v, p);
		free(p);
	}
	push(getStack(code->dotcnt), v);
}

void Op_Mul (struct Code *code, struct Heart_Tree *tree)
{
	struct Value *v = pop(stack_current), *p;
	for (int i = 1; i < code->charcnt; i++)
	{
		p = pop(stack_current);
		v = ValueMul(v, p);
		free(p);
	}
	push(getStack(code->dotcnt), v);
}

void Op_Neg (struct Code *code, struct Heart_Tree *tree)
{
	struct Value *v = getNewValue(), *p;
	struct Stack *stack_temp = getStack(-1); // get temp stack

	for (int i = 0; i < code->charcnt; i++)
	{
		p = pop(stack_current);
		ValueNeg(p);
		v = ValueAdd(v, p);
		push(stack_temp, p);
	}
	for (int i = 0; i < code->charcnt; i++)
		push(stack_current, pop(stack_temp));

	push(getStack(code->dotcnt), v);
	free(stack_temp);
}

void Op_Inv (struct Code *code, struct Heart_Tree *tree)
{
	struct Value *v = getNewValue(), *p;
	struct Stack *stack_temp = getStack(-1); // get temp stack

	v->top = 1;
	for (int i = 0; i < code->charcnt; i++)
	{
		p = pop(stack_current);
		ValueInv(p);
		v = ValueMul(v, p);
		push(stack_temp, p);
	}
	for (int i = 0; i < code->charcnt; i++)
		push(stack_current, pop(stack_temp));

	push(getStack(code->dotcnt), v);
	free(stack_temp);
}

void Op_Copy (struct Code *code, struct Heart_Tree *tree)
{
	struct Value *v = pop(stack_current);
	struct Stack *stack_target = getStack(code->dotcnt);

	for (int i = 0; i < code->charcnt; i++)
	{
		push(stack_target, ValueCopy(v));
	}
	push(stack_current, v);
	stack_current = stack_target;
}


void (*operator[OP_LENGTH])(struct Code *, struct Heart_Tree *) = {NULL,
	&Op_Push, &Op_Add, &Op_Mul, &Op_Neg, &Op_Inv, &Op_Copy
};

void interpret(struct Code *code, int debug, int max_code)
{
	// create default stack
	stack_stdin  = Stack_Hash[0] = malloc(sizeof(struct Stack));
	stack_stdout = Stack_Hash[1] = malloc(sizeof(struct Stack));
	stack_stderr = Stack_Hash[2] = malloc(sizeof(struct Stack));
	stack_current = Stack_Hash[3] = malloc(sizeof(struct Stack));

	*stack_stdin = *stack_stdout = *stack_stderr = *stack_current = STACK_NULL;
	stack_stdin->stack_value = 0;
	stack_stdout->stack_value = 1;
	stack_stderr->stack_value = 2;
	stack_current->stack_value = 3;

	while (1)
	{
		if (debug) {
			print_debug_info(max_code, code);
			print_stack_info(code);
		}

		(*operator[code->opcode])(code, NULL);
		if (code->tree) (*operator[code->tree->opcode])(code, code->tree);

		code = code->next;
	}
}