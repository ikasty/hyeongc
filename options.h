#ifndef OPTIONS_H
#define OPTIONS_H

struct Options{
	int debug, parseonly;
	int max_len;
	char *filename;
	void *last_push;
	int pop_stack;
};

extern struct Options options;

#endif