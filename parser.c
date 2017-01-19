#include <stdio.h>
#include <stdlib.h>

#include "code.h"
#include "token.h"
#include "parser.h"
#include "parser_state.h"

struct Code *code_head, *code_before;
struct State state;

struct Code *parse (int *codenum, char *filename)
{
	FILE *ifp = fopen(filename, "r");
	if (ifp == NULL) return NULL;

	code_head = state.code = malloc(sizeof(struct Code));
	*state.code = CODE_NULL;

	state.token = getToken(ifp);

	*codenum = 0;
	while (state.token)
	{
		Parse_token(&state);
		if (!state.code->opcode) continue;
		state.code->code_num = ++*codenum;
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

	return code_head;
}
