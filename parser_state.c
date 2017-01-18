#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "code.h"
#include "parser_state.h"

void consume (struct State *s)
{
	struct Token *token = s->token;
	s->token = s->token->next;
	free(token);
}

int isStartKorean (struct Token *t)
{
	switch (t->value)
	{
	case NUM_PUSH: case NUM_PUSH_START: case ADD: case MUL: case NEG:
	case INV: case COPY: case ADD_MUL: case NEG_INV_COPY:
		return 1;
	default:
		return 0;
	}
}

int isEndKorean (struct Token *t)
{
	switch (t->value)
	{
	case NUM_PUSH_END: case ADD_END: case MUL_END: case NEG_END:
	case INV_END: case COPY_END:
		return 1;
	default:
		return 0;
	}
}

void Parse_token (struct State *s)
{
	while (1)
	{
		if (s->token->value == SPACE) return consume(s);
		if (!s->token) return ;
		if (!isStartKorean(s->token)) consume(s);
		else break;
	}

	switch (s->token->value)
	{
	case NUM_PUSH_START: case ADD_MUL: case NEG_INV_COPY:
		s->code->charcnt = 1;
		s->flag = s->token->value;
		consume(s);
		return Parse_Korean(s);

		case NUM_PUSH: s->code->opcode = OP_PUSH; break;
		case ADD: s->code->opcode = OP_ADD; break;
		case MUL: s->code->opcode = OP_MUL; break;
		case NEG: s->code->opcode = OP_NEG; break;
		case INV: s->code->opcode = OP_INV; break;
		case COPY: s->code->opcode = OP_COPY; break;
	}
	s->code->charcnt = 1;
	consume(s);
	return Parse_dot(s);
}

void Parse_Korean (struct State *s)
{
	while (1)
	{
		if (s->token->value == SPACE) return consume(s);
		if (!s->token) return ;
		if (isEndKorean(s->token)) break;

		if (s->token->value != STR) consume(s);
		else {
			s->code->charcnt++;
			consume(s);
		}
	}

	s->code->charcnt++;
	enum Token_Value token = s->token->value;
	consume(s);
	switch (token)
	{
	case NUM_PUSH_END:
		if (s->flag != NUM_PUSH_START) return Parse_Korean(s);
		s->code->opcode = OP_PUSH;
		break;

	case ADD_END: case MUL_END:
		if (s->flag != ADD_MUL) return Parse_Korean(s);
		if (token == ADD_END) s->code->opcode = OP_ADD;
		if (token == MUL_END) s->code->opcode = OP_MUL;
		break;
	
	case NEG_END: case INV_END: case COPY_END:
		if (s->flag != NEG_INV_COPY) return Parse_Korean(s);
		if (token == NEG_END)  s->code->opcode = OP_NEG;
		if (token == INV_END)  s->code->opcode = OP_INV;
		if (token == COPY_END) s->code->opcode = OP_COPY;
		break;
	}

	s->flag = 0;
	return Parse_dot(s);
}

int isHeartSection (struct Token *t)
{
	switch (t->value)
	{
		case QUES: case BANG: case HEART: case REF: return 1;
		default: return 0;
	}
}

int isHeart (struct Token *t)
{
	return (t->value == HEART) || (t->value == REF);
}

void Parse_dot (struct State *s)
{
	while (1)
	{
		if (s->token->value == SPACE) return consume(s);
		if (!s->token) return ;
		if (isStartKorean(s->token)) return ;
		if (isHeartSection(s->token)) {
			s->code->tree = Parse_Ques(s);
			return ;
		}

		if (s->token->value == DOT) {
			s->code->dotcnt += s->token->subvalue;
		}

		consume(s);
	}
}

struct Heart_Tree *getHeart (struct State *s)
{
	while (!isHeartSection(s->token) && s->token->value != SPACE)
		consume(s);

	if (s->token->value == SPACE) {
		consume(s);
		return NULL;
	}

	if (s->token->value == BANG || s->token->value == QUES) return NULL;
	
	struct Heart_Tree *heart = malloc(sizeof(struct Heart_Tree));
	*heart = HEART_TREE_NULL;

	if (s->token->value == HEART) heart->opcode = OP_SAVE;
	else heart->opcode = OP_REF;
	heart->value = s->token->subvalue;
	heart->code = s->code;

	while (isHeart(s->token)) consume(s);

	return heart;
}

struct Heart_Tree *Parse_Bang (struct State *s)
{
	struct Heart_Tree *heart = getHeart(s);
	if (s->token->value == BANG) {
		struct Heart_Tree *tree = malloc(sizeof(struct Heart_Tree));
		*tree = HEART_TREE_NULL;

		tree->opcode = OP_COMPZ;
		tree->left = heart;
		tree->right = Parse_Bang(s);
		heart = tree;
	}

	return heart;
}

struct Heart_Tree *Parse_Ques (struct State *s)
{
	struct Heart_Tree *heart = Parse_Bang(s);
	if (s->token->value == QUES) {
		struct Heart_Tree *tree = malloc(sizeof(struct Heart_Tree));
		*tree = HEART_TREE_NULL;

		tree->opcode = OP_COMPL;
		tree->left = heart;
		tree->right = Parse_Ques(s);
		heart = tree;
	}

	return heart;
}
