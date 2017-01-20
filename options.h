#ifndef OPTIONS_H
#define OPTIONS_H

struct Options{
	int debug, parseonly;
	int max_len;
	char *filename;
};

extern struct Options options;

#endif