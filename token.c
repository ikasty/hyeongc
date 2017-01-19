#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "token.h"

int isKoreanUnicode (int32_t);
int getUlen (char *);
int32_t getUnicode (char *);

struct Token *getToken (FILE *ifp)
{
	int offset = 0;
	char inputs[INPUT_BUFFER];

	struct Token *tkn, *token_head;
	int ccount = 0;

	tkn = token_head = malloc(sizeof(struct Token));
	*token_head = TOKEN_NULL;

	while (fgets(inputs+offset, INPUT_BUFFER-offset, ifp) != NULL)
	{
		char *p = inputs;
		int i = 0;
		offset = 0;
		while (*p)
		{
			int len = getUlen(p);
			if (i+len >= INPUT_BUFFER) {
				for (offset = 0; *p; offset++, p++) inputs[offset] = *p;
				break;
			}
			i+=len;

			int32_t unicode = getUnicode(p);
			p+=len;

			switch (unicode)
			{
			case 0xD615: tkn->value = NUM_PUSH; break; // 형
			case 0xD600: tkn->value = NUM_PUSH_START; break; // 혀
			case 0xC5C9: tkn->value = NUM_PUSH_END; break; // 엉
			case 0xD56D: tkn->value = ADD; break; // 항
			case 0xD56B: tkn->value = MUL; break; // 핫
			case 0xD763: tkn->value = NEG; break; // 흣
			case 0xD761: tkn->value = INV; break; // 흡
			case 0xD751: tkn->value = COPY; break; // 흑
			case 0xD558: tkn->value = ADD_MUL; break; // 하
			case 0xD750: tkn->value = NEG_INV_COPY; break; // 흐
			case 0xC559: tkn->value = ADD_END; break; // 앙
			case 0xC557: tkn->value = MUL_END; break; // 앗
			case 0xC74F: tkn->value = NEG_END; break; // 읏
			case 0xC74D: tkn->value = INV_END; break; // 읍
			case 0xC73D: tkn->value = COPY_END; break; // 윽
			case '.': tkn->value = DOT; tkn->subvalue = 1; break;
			case '?': tkn->value = QUES; break;
			case '!': tkn->value = BANG; break;
			case 0x2026: case 0x22EF: case 0x22EE:
				tkn->value = DOT; tkn->subvalue = 3; break; // …, ⋯, ⋮
			case 0x2665: case 0x2764: // ♥, ❤
				tkn->value = HEART;
				if (unicode == 0x2665) tkn->subvalue = 1;
				else tkn->subvalue = 2;
				break;
			case 0x1F495: case 0x1F496: case 0x1F497: // 💕, 💖, 💗
			case 0x1F498: case 0x1F499: case 0x1F49A: // 💘, 💙, 💚
			case 0x1F49B: case 0x1F49C: case 0x1F49D: // 💛, 💜, 💝
				tkn->value = HEART;
				tkn->subvalue = unicode - 0x1F495 + 3;
				break;
			case 0x2661: tkn->value = REF; break;// ♡
			case ' ': case '\t': case '\r': case '\n':
				tkn->value = SPACE; break;
			default:
				if (isKoreanUnicode(unicode)) tkn->value = STR;
				else tkn->value = NUL;
			}
			tkn->unicode = unicode;

			tkn = tkn->next = malloc(sizeof(struct Token));
			*tkn = TOKEN_NULL;
		}
	}

	return token_head;
}

int isKoreanUnicode (int32_t unicode) {
	return unicode >= 0xAC00 && unicode <= 0xD7A3;
}

int getUlen (char *p)
{
	int flag = 0x80, len;
	for (len = *p & flag ? 0:1; *p & flag; flag>>=1, len++);
	return len;
}

int32_t getUnicode (char *p)
{
	int32_t res = 0;
	unsigned char *x = (unsigned char*)p;
	int len = getUlen(x);
	switch (len) {
	case 1:
		res = *x; break;
	case 2:
		res = (x[0] & 0300) << 6 | (x[1] - 0200);
		break;
	case 3:
		res = (x[0] - 0340) << 12 | (x[1] - 0200) << 6 | (x[2] - 0200);
		break;
	case 4:
		res = (x[0] - 0360) << 18 | (x[1] - 0200) << 12;
		res|= (x[2] - 0200) << 6 | (x[3] - 0200);
		break;
	}
	return res;
}
