#ifndef DEBUG_H
#define DEBUG_H

#include "code.h"
#include "interpreter.h"

void print_debug_start ();
void print_debug_info (struct Code *);
void print_stack_info (struct Code *, struct Stack *[10]);

void print_value_start ();
void print_value_end ();
void print_program_end(int);

#endif
