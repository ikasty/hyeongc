#include <stdio.h>
#include "debug.h"
#include "code.h"
#include "options.h"

char *codestr[OP_LENGTH] = {"",
	"형", "항", "핫", "흣", "흡", "흑", "❤", "♡", "?", "!"
};

char *heartstr[] = {"",
	"♥", "❤", "💕", "💖", "💗", "💘", "💙", "💚", "💛", "💜", "💝"
};

void print_tree (struct Heart_Tree *t)
{
	if (!t) {putchar('_'); return ;}

	if (t->opcode == OP_SAVE) printf("%s", heartstr[t->value]);
	else if (t->opcode == OP_REF) printf("%s", codestr[t->opcode]);
	else {
		printf("%s", codestr[t->opcode]);
		print_tree(t->left);
		print_tree(t->right);
	}
}

void print_debug_info (struct Code *code)
{
	int len;
	int max_len = options.max_len;
	for (len = 0; max_len; max_len /= 10, len++);

	printf("[%*d] ", len, code->code_num);
	printf("%s %d %d", codestr[code->opcode], code->charcnt, code->dotcnt);
	if (code->tree) putchar(' '), print_tree(code->tree);
	puts("");
}

void print_stack_info (struct Code *code, struct Stack *Stack_Hash[10])
{
	printf("\n스택 정보:\n");
	for (int i = 0; i < 10; i++)
	{
		struct Stack *stack_debug = Stack_Hash[i];
		while (stack_debug)
		{
			if (stack_debug->stack_value == 0 ||
			    stack_debug->stack_value == 1 ||
			    stack_debug->stack_value == 2)
			{
				stack_debug = stack_debug->next;
				continue;
			}
			printf("\t%d번 스택: ->", stack_debug->stack_value);

			struct Value *v = stack_debug->value;
			while (v)
			{
				printf(" (%lld/%lld)", v->top, v->bottom * !v->nan);
				v = v->stackp;
			}
			puts("");

			stack_debug = stack_debug->next;
		}
	}
}