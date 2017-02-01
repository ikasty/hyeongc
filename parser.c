#include <stdio.h>
#include <stdlib.h>

#include "code.h"
#include "token.h"
#include "parser.h"
#include "parser_state.h"
#include "options.h"

struct Code *code_head, *code_before;
struct State state;

struct Code *parse (char *filename)
{
	int codenum;
	FILE *ifp = fopen(filename, "r");
	if (ifp == NULL) return NULL;

	code_head = state.code = malloc(sizeof(struct Code));
	*state.code = CODE_NULL;

	state.token = getToken(ifp);

	codenum = -1;
	while (state.token)
	{
		Parse_token(&state);
		if (!state.code->opcode) continue;
		state.code->code_num = ++codenum;
		if (!state.token) break;

		code_before = state.code;
		state.code = state.code->next = malloc(sizeof(struct Code));
		*state.code = CODE_NULL;
	}

	if (!state.code->opcode) {
		free(state.code);
		state.code = code_before;
	}
	state.code->next = code_head;
	options.max_len = codenum;

	return code_head;
}
