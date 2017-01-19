#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "interpreter.h"
#include "debug.h"

#define LANG_VER "v0.4.4"
#define COMP_VER "v0.4.4"

struct {
	int debug, parseonly;
	char *filename;
} options;

void help (int code)
{
	printf(
		" 난해한 혀엉...언어 " LANG_VER " 구현체\n"
		" 혀엉씨 " COMP_VER "\n\n"

		" 사용법: hyeongc [options] [--] 파일명\n\n"

		"\t-v, --version,\n"
		"\t-h, --help     현재 메시지를 출력합니다.\n"
		"\t-d, --debug    디버그 메시지를 출력합니다.\n"
		"\t--parse-only   실행하지 않고, 파싱 결과만을 출력합니다.\n"
	);

	exit(code);
}

void getOptions (int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-') break;
		if (!strcmp(argv[i], "--")) {i++; break;}

		if (!strcmp(argv[i], "-d")) options.debug = 1;
		if (!strcmp(argv[i], "-v")) return help(0);
		if (!strcmp(argv[i], "-h")) return help(0);

		if (!strcmp(argv[i], "--debug"))		options.debug = 1;
		if (!strcmp(argv[i], "--version"))		return help(0);
		if (!strcmp(argv[i], "--help"))			return help(0);
		if (!strcmp(argv[i], "--parse-only"))	options.parseonly = 1;
	}
	options.filename = argv[i];
}

int main (int argc, char **argv)
{
	getOptions(argc, argv);

	if (!options.filename) {
		printf("에러: 파일명이 주어지지 않았습니다.\n");
		help(1);
	}

	int max_code;
	struct Code *code = parse(&max_code, options.filename);
	if (options.parseonly) {
		struct Code *p = code;
		do
		{
			print_debug_info(max_code, p);
			p = p->next;
		} while (p != code);
	}
	else interpret(code, options.debug, max_code);

	return 0;
}