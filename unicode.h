#ifndef UNICODE_H
#define UNICODE_H

#include <inttypes.h>

int isKoreanUnicode (int32_t);
int getUTF8Len (char *);
int32_t getUnicode (char *);
char *getUTF8 (int32_t);

#endif
