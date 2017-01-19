#ifndef TOKEN_H
#define TOKEN_H

#include <inttypes.h>
#include <stdio.h>
#define INPUT_BUFFER 80

enum Token_Value {
	SPACE = 0, // 공백
	NUM_PUSH = 1, ADD, MUL, NEG, INV, COPY, // 형, 혀, 항, 핫, 흣, 흡, 흑
	NUM_PUSH_START = 10, ADD_MUL, NEG_INV_COPY, // 혀, 하, 흐
	NUM_PUSH_END = 30, ADD_END, MUL_END, NEG_END, INV_END, COPY_END, // 엉, 앙, 앗, 읏, 읍, 윽
	DOT = 50, QUES, BANG, // ., ?, !
	HEART = 100, REF, // ❤, ♡
	STR = 200, NUL = 1000 // 한글 음절 문자, 기타 문자
};

struct Token {
	enum Token_Value value;
	int subvalue;
	struct Token *next;
	int32_t unicode;
} TOKEN_NULL;

struct Token *getToken (FILE *);

#endif