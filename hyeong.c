#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "interpreter.h"

struct {
	int debug;
	char *filename;
} options;

void version ()
{
	printf(
		" 난해한 혀엉...언어 v0.4.3 구현체\n"
		" 혀엉씨 v0.4.3\n"
	);

	exit(0);
}

void error (int err, char *message)
{
	if (err) {
		printf("잘못된 옵션이 있습니다");
		if (message) printf(": %s", message);
		printf("\n");
	}

	printf("사용법: hyeong {파일명}\n");
	exit(1);
}

void getOptions (int argc, char **argv)
{
	int current = 1;
	int mode = 0;
	char *p = argv[current];

	while (current < argc)
	{
		switch (*p)
		{
		case '\0':
			current++;
			p=argv[current];
			if (mode == 3) mode = 0; // '-- filename option'
			else if (mode == 1) return error(1, "옵션이 주어지지 않았습니다");
			else if (mode == 2) mode = 0;
			continue;
		case '-':
			if (!mode) { mode = 1; break; }
			else if (mode) { mode = 3; break; }

		case 'd':
			if (mode && mode < 3) { options.debug = 1; mode = 2; }
			break;

		case 'v':
			if (mode && mode < 3) version();
		}
		if (!mode) { options.filename = p; return ; }
		p++;
	}
}

int main (int argc, char **argv)
{
	getOptions(argc, argv);

	if (!options.filename) error(0, NULL);

	struct Code *code = parse(options.filename);
	interpret(code, options.debug);

	return 0;
}