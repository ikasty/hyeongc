#include <stdio.h>
#include <stdlib.h>

#include "code.h"
#include "token.h"
#include "parser.h"
#include "parser_state.h"

struct Code *code_head;
struct State state;

struct Code *parse (char *filename)
{
	FILE *ifp = fopen(filename, "r");
	if (ifp == NULL) return NULL;

	code_head = state.code = malloc(sizeof(struct Code));
	*state.code = CODE_NULL;

	state.token = getToken(ifp);

	while (state.token)
	{
		Parse_token(&state);
		if (!state.code->opcode) continue;
		state.code = state.code->next = malloc(sizeof(struct Code));
		*state.code = CODE_NULL;
	}
	state.code->next = code_head;

	return code_head;
}
