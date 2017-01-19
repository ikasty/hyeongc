#include <stdio.h>
#include "debug.h"

char *codestr[OP_LENGTH] = {"",
	"형", "항", "핫", "흣", "흡", "흑"
	"", "♡", "?", "!"
};

char *heartstr[] = {"",
	"♥", "❤", "💕", "💖", "💗", "💘", "💙", "💚", "💛", "💜", "💝"
};

void print_tree (struct Heart_Tree *t)
{
	if (t->opcode == OP_SAVE) printf("%s", heartstr[t->value]);
	else printf("%s", codestr[t->opcode]);

	if (t->left) print_tree(t->left);
	if (t->right) print_tree(t->right);
}

void print_debug_info (int max_len, struct Code *code)
{
	printf("[%*d] ", max_len, code->code_num);
	printf("%s %d %d", codestr[code->opcode], code->charcnt, code->dotcnt);
	print_tree(code->tree);
	puts("");
}

void print_stack_info (struct Code *code)
{
	printf("\n스택 정보:\n");
	for (int i = 0; i < 10; i++)
	{
		struct Stack *stack_debug = Stack_Hash[i];
		while (stack_debug)
		{
			if (stack_debug == stack_stdin ||
			    stack_debug == stack_stdout ||
			    stack_debug == stack_stderr)
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