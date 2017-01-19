#ifndef PARSER_STATE_H
#define PARSER_STATE_H

#include "code.h"
#include "token.h"

struct State {
	struct Code *code;
	struct Token *token;
	enum Token_Value flag;
};

// init state
void Parse_token (struct State *);
void Parse_End_Korean (struct State *);
void Parse_dot (struct State *);
struct Heart_Tree *Parse_Ques(struct State *);

#endif
