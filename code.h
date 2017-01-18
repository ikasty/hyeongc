#ifndef CODE_H
#define CODE_H

enum OPCODE {
	OP_PUSH = 1, OP_ADD, OP_MUL, OP_NEG, OP_INV, OP_COPY,
	OP_SAVE, OP_REF,
	OP_COMPL, OP_COMPZ,

	OP_LENGTH
};

struct Code;

struct Heart_Tree {
	enum OPCODE opcode;
	int value;
	struct Code *code;
	struct Heart_Tree *left, *right;
} HEART_TREE_NULL;

struct Code {
	enum OPCODE opcode;
	int code_num;
	int charcnt, dotcnt;
	struct Code *next;
	struct Heart_Tree *tree;
} CODE_NULL;

#endif
