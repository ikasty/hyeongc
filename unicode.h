#ifndef UNICODE_H
#define UNICODE_H

#include <stdio.h>
#include <inttypes.h>

int isKoreanUnicode (int32_t);
int getUTF8Len (char *);
int32_t getUnicode (char *);
char *getUTF8 (int32_t);
int32_t getcUnicode (char *, const int, int *, FILE *);

#endif
