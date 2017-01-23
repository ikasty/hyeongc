#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "unicode.h"
#include "token.h"

struct Token *getToken (FILE *ifp)
{
	char buffer[INPUT_BUFFER] = {0};
	int pos = 0;

	struct Token *tkn, *token_head;
	tkn = token_head = malloc(sizeof(struct Token));
	*token_head = TOKEN_NULL;

	int32_t unicode;
	while (unicode = getcUnicode(buffer, INPUT_BUFFER, &pos, ifp))
	{
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

	return token_head;
}
