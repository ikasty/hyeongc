#include <stdio.h>
#include "parser.h"
#include "interpreter.h"

int main (int argc, char** argv)
{
	if (argc < 2) return -1;

	struct Code *code = parse(argv[1]);
	interpret(code);

	return 0;
}