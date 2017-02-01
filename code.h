#ifndef CODE_H
#define CODE_H

enum OPCODE {
	OP_PUSH = 1, OP_ADD, OP_MUL, OP_NEG, OP_INV, OP_COPY,
	OP_SAVE, OP_REF,
	OP_COMPL, OP_COMPZ,

	OP_LENGTH
};

struct Value {
	int nan;
	long long top, bottom;
	struct Value *stackp;
} VALUE_NULL;

struct Code;

struct Heart_Tree {
	enum OPCODE opcode;
	int value;
	struct Heart_Tree *left, *right;
} HEART_TREE_NULL;

struct Code {
	enum OPCODE opcode;
	int code_num;
	int charcnt, dotcnt;
	struct Code *next;
	struct Heart_Tree *tree;
} CODE_NULL;

struct Pointers {
	int tag;
	struct Code *code;
	struct Pointers *next;
} POINTER_NULL;

struct Stack {
	int stack_value;
	struct Stack *next;
	struct Value *value;
} STACK_NULL;

#endif
