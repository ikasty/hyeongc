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

void print_debug_start ()
{
	printf("프로그램을 실행합니다 (아무 키나 누르세요):");
}

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

	fflush(stdout);
	printf("[%*d] ", len, code->code_num);
	printf("%s %d %d", codestr[code->opcode], code->charcnt, code->dotcnt);
	if (code->tree) putchar(' '), print_tree(code->tree);
	puts("");
}

void print_stack_info (	struct Code *code,
						struct Stack *stack_current,
						struct Stack *Stack_Hash[10])
{
	printf("현재 스택: %d\n", stack_current->stack_value);
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
			if (stack_debug->stack_value == options.pop_stack) {
				printf("\033[1;31m");
				options.pop_stack = 0;
			}
			printf("\t%d번 스택: ->", stack_debug->stack_value);
			printf("\033[0m");

			struct Value *v = stack_debug->value;
			while (v)
			{
				if (v == options.last_push) {
					printf("\033[1;33m");
					options.last_push = NULL;
				}
				if (v->nan) printf(" (NaN)");
				else printf(" (%lld/%lld)", v->top, v->bottom);
				printf("\033[0m");
				v = v->stackp;
			}
			puts("");

			stack_debug = stack_debug->next;
		}
	}
}

void print_value_start ()
{
	puts("출력 내용: \033[1;36m");
}

void print_value_end ()
{
	puts("\033[0m\n");
}

void print_program_end(int endcode)
{
	puts("\033[0m");
	if (endcode) puts("\n\n프로그램이 비정상적으로 종료되었습니다\n");
	else puts("\n\n프로그램이 정상적으로 종료되었습니다\n");
}
