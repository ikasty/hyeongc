#ifndef TOKEN_H
#define TOKEN_H

#include <inttypes.h>
#include <stdio.h>
#define INPUT_BUFFER 80

enum Token_Value {
	SPACE = 0, // 공백
	NUM_PUSH = 1, NUM_PUSH_START, NUM_PUSH_END, // 형, 혀, 엉
	ADD, MUL, NEG, INV, COPY, // 항, 핫, 흣, 흡, 흑
	ADD_MUL, NEG_INV_COPY, // 하, 흐
	ADD_END, MUL_END, NEG_END, INV_END, COPY_END, // 앙, 앗, 읏, 읍, 윽
	DOT, QUES, BANG, // ., ?, !
	HEART, REF, // ❤, ♡
	STR, NUL // 한글 음절 문자, 기타 문자
};

struct Token {
	enum Token_Value value;
	int subvalue;
	struct Token *next;
	int32_t unicode;
} TOKEN_NULL;

struct Token *getToken (FILE *);

#endif