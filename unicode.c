#include <stdlib.h>
#include <inttypes.h>

#include "unicode.h"

int isKoreanUnicode (int32_t unicode) {
	return unicode >= 0xAC00 && unicode <= 0xD7A3;
}

int getUTF8Len (char *p)
{
	int flag = 0x80, len;
	for (len = *p & flag ? 0:1; *p & flag; flag>>=1, len++);
	return len;
}

int32_t getUnicode (char *p)
{
	int32_t res = 0;
	unsigned char *x = (unsigned char*)p;
	int len = getUTF8Len(x);
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

int getUnicodeLen (int32_t unicode)
{
	int b = 1;
	if (unicode >= 0x80) b++;
	if (unicode >= 0x800) b++;
	if (unicode >= 0x10000) b++;
	return b;
}

char *getUTF8 (int32_t unicode)
{
	char *utf8 = calloc(4, sizeof(char));
	char lastmark = 0xC0; // 1100 0000
	switch (getUnicodeLen(unicode))
	{
	case 1:
		utf8[0] = (char)unicode;
		break;

	case 4:
		utf8[3] = unicode & 077 | 0200;
		unicode >>= 6;
		lastmark >>= 1;
	case 3:
		utf8[2] = unicode & 077 | 0200;
		unicode >>= 6;
		lastmark >>= 1;
	case 2:
		utf8[1] = unicode & 077 | 0200;
		unicode >>= 6;
		utf8[0] = (char)unicode | lastmark;
		break;
	}

	return utf8;
}
